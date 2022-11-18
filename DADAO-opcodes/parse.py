def is_binary_str(item: str):
    for i in item:
        if i != '0' and i != '1':
            return False
    return True

def string_tab(string, len):
    assert len(string) < len
    string = string + ' ' * (len - len(string))
    return string

def field_bincode(field, cond):
    bincode = None
    for rest in cond:
        if rest.find(field) != -1 and rest.find('==') != -1:
            bincode = rest[rest.find('==') + 2:]
            break
    return bincode

def regfile_fillin(field, regfile_restrictions):
    regname = field
    if field in regfile_restrictions.keys():
        regname = regfile_restrictions[field]
    return regname

def get_mask_match(fields, opcode, cond_restrictions):
    mask = ''
    match = ''
    for field in fields:
        if field == 'op':
            mask += '1' * fields[field]
            match += opcode
        elif field_bincode(field, cond_restrictions) != None:
            mask += '1' * fields[field]
            match += field_bincode(field, cond_restrictions)
        else:
            mask += '0' * fields[field]
            match += '0' * fields[field]
    return mask, match

def parse_line(line: str):
    if line[0] == '#':
        return None
    if line[0] == '[':
        line_type = 'inst_type'
        info = line.strip().split()
        info = [item for item in info if item != '[' and item != ']']
        optype = info[0]
        fields = {}
        for field in info[1:]:
            field_name, field_len = field.split(':')
            fields[field_name] = int(field_len)
        return line_type, optype, fields
    if line[0] == '&':
        line_type = 'restriction_type'
        info = line[1:].strip().split()
        resources = []
        regfile_restrictions = {}
        cond_restrictions = []
        for item in info:
            if item[0] == '@':
                resources.append(item[1:])
            elif '@' in item and item[0] != '@':
                [regfile, field] = item.strip().split('@')
                regfile_restrictions[field] = regfile
            elif item[0] == '(':
                cond_restrictions.append(item[1:-1])
            elif item[0] == '$':
                insn_type = item[1:]
        return  line_type, resources, regfile_restrictions, cond_restrictions
    if line != '' and len(line.strip().split()) > 1:
        line_type = 'inst'
        info = line.strip().split()
        inst_name = info[0]
        opcode = info[1]
        resources = []
        regfile_restrictions = {}
        cond_restrictions = []
        if len(info) < 3:
            return line_type, inst_name, opcode, resources, regfile_restrictions, cond_restrictions
        if is_binary_str(info[2]):
            opcode += info[2]
        if '&&' in info:
            tmp = info.index('&&')
            for item in info[tmp+1:]:
                if item[0] == '@':
                    resources.append(item[1:])
                elif '@' in item and item[0] != '@':
                    [regfile, field] = item.strip().split('@')
                    regfile_restrictions[field] = regfile
                elif item[0] == '(':
                    cond_restrictions.append(item[1:-1])
        return line_type, inst_name, opcode, resources, regfile_restrictions, cond_restrictions
    return None
        
def read_opcodes(file_name: str):
    insts = {}
    bincodes = {}
    with open(file_name) as f:
        while True:
            line = f.readline()
            if not line:
                break
            ret = parse_line(line)
            if ret == None:
                continue
            line_type = ret[0]
            if line_type == 'inst_type':
                now_optype, now_fields = ret[1:]
            if line_type == 'restriction_type':
                now_resources, now_regfile_restrictions, now_cond_restrictions = ret[1:]
            if line_type == 'inst':
                inst_name, opcode, resources, regfile_restrictions, cond_restrictions = ret[1:]
                resources += now_resources
                for key, value in now_regfile_restrictions.items():
                    if key in regfile_restrictions.keys():
                        regfile_restrictions[key] += value
                    else:
                        regfile_restrictions[key] = value
                cond_restrictions += now_cond_restrictions
                if inst_name in insts.keys():
                    inst_name = '_' + inst_name

                if 'mem' in resources:
                    if 'jmp' in resources:
                        insn_type = 'jsr'
                    else:
                        insn_type = 'dref'
                elif 'jmp' in resources:
                    if 'bpd' in resources:
                        insn_type = 'condbranch'
                    else:
                        insn_type = 'branch'
                else:
                    insn_type = 'normal'
                
                mask, match = get_mask_match(now_fields, opcode, cond_restrictions)
                mask_int = int(mask, base=2)
                match_int = int(match, base=2)
                insts[inst_name] = {
                    'optype': now_optype,
                    'fields': now_fields,
                    'opcode': opcode,
                    'resources': resources,
                    'regfile_restrictions': regfile_restrictions,
                    'cond_restrictions': cond_restrictions,
                    'insn_type': insn_type,
                    'mask': mask,
                    'match': match,
                    'mask_int': mask_int,
                    'match_int': match_int
                }
        return insts



def gen_decode_file(insts: dict, output_file: str):
    ## problem: orw* should use different binary code to define
    ## solution: use field_bincode to fill the fields that can be known
    ## problem: ret is covered in call
    ## solution: check ret instructions separately
    with open(output_file, 'w') as f:
        print('''# This file is processed by scripts/decodetree.py


        ''', file=f)
        for inst_name, inst_description in insts.items():
            if inst_name == 'ret':  ## not very good？
                continue
            line = inst_name + ' ' * (10 - len(inst_name))
            if len(inst_description['opcode']) == 8:
                line = line + inst_description['opcode'] + ' '
            elif len(inst_description['opcode']) == 14:
                line = line + inst_description['opcode'][:8] + ' '\
                          + inst_description['opcode'][8:] + ' '
            for field_name, field_len in inst_description['fields'].items():
                if field_name == 'op':
                    continue
                code = field_bincode(field_name, inst_description['cond_restrictions'])
                if code != None:
                    line += code + ' '
                else:
                    line = line + field_name + ':' + str(field_len) + ' '
            print(line, file=f)
        return
    


def gen_opc_file(insts: dict, output_file: str):
    with open(output_file, 'w') as f:
        for inst_name, inst_description in insts.items():
            fields = list(inst_description['fields'].keys())[1:]
            regfile_restrictions = inst_description['regfile_restrictions']
            operands = fields + ['none'] * (4 - len(fields))
            operands = [regfile_fillin(operands[i], regfile_restrictions) for i in range(len(operands))]
            majop = hex(int(inst_description['opcode'][:8], base=2))
            if len(inst_description['opcode']) == 8:
                minop = 0
            else:
                minop = hex(int(inst_description['opcode'][8:], base=2))
            line = '   {{ "{}", {}, {}, {}, _OP({}), _OP({}), _OP({}), _OP({}), _T({})}},'\
                .format(inst_name, len(fields), majop, minop,
                operands[0], operands[1], operands[2], operands[3], inst_description['insn_type'])
            print(line, file=f)
        return 

def gen_encoding_file(insts: dict, output_file: str):
    mask_match_str = ''
    declare_insn_str = ''
    for i in insts:
        mask_match_str += f'#define MATCH_{i.upper().replace(".","_")} {hex(insts[i]["match_int"])}\n'
        mask_match_str += f'#define MASK_{i.upper().replace(".","_")} {hex(insts[i]["mask_int"])}\n'
        declare_insn_str += f'DECLARE_INSN({i.replace(".","_")}, MATCH_{i.upper().replace(".","_")}, MASK_{i.upper().replace(".","_")})\n'

    with open(output_file, 'w') as f:
        print(mask_match_str, file=f)
        print(declare_insn_str, file=f)








if __name__ == '__main__':
    import os
    import sys
    insts = read_opcodes(sys.argv[1])
    gen_decode_file(insts, sys.argv[2])
    gen_opc_file(insts, sys.argv[3])
    gen_encoding_file(insts, sys.argv[4])

