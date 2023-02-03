#!/usr/bin/python3

def help_information():
    print("Usage: parse-opcodes.py [options] file...\n")
    print("Options:")
    print("--help\t\t\tDisplay this information.\n")
    print("--input\t\t\tDefine opcodes file's input path.\n")
    print("--decode\t\tDefine decode file's output path.\n")
    print("--opc\t\t\tDefine opc file's output path.\n")
    print("--encoding\t\tDefine encoding file's output path.\n")
    print("--disassemble\t\tDefine disassemble file's output path.\n")
    print("--bitpat\t\tDefine bitpat file's output path.\n")
    print("--dasm dadao\t\tRun dadao-dasm module.")
    print("\t\t\tdasm format: DASM(inst)\n")

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

def append_immlen(field, field_len):
    return field + str(field_len)

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
            if field_name[0:3] == 'imm':
                field_name = append_immlen(field_name, int(field_len))
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
                        if 'hb' not in regfile_restrictions:
                            insn_type = 'condbranch'
                        else:
                            insn_type = 'condbranch12'
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
    with open(output_file, 'w') as f:
        print('''# This file is processed by scripts/decodetree.py


        ''', file=f)
        for inst_name, inst_description in insts.items():
            symbol = ''
            line = inst_name + ' ' * (10 - len(inst_name))
            if len(inst_description['opcode']) == 8:
                line = line + inst_description['opcode'] + ' '
            elif len(inst_description['opcode']) == 14:
                line = line + inst_description['opcode'][:8] + ' '\
                          + inst_description['opcode'][8:] + ' '
            for field_name, field_len in inst_description['fields'].items():
                if field_name == 'op':
                    continue
                if field_name[0:4] == 'imms':
                    symbol = 's'
                code = field_bincode(field_name, inst_description['cond_restrictions'])
                if code != None:
                    line += code + ' '
                else:
                    line = line + field_name + ':' + symbol + str(field_len) + ' '
            print(line, file=f)
        return
    


def gen_opc_file(insts: dict, output_file: str):
    with open(output_file, 'w') as f:
        for inst_name, inst_description in insts.items():
            if inst_description['fields']['op'] == 8:
                fields = list(inst_description['fields'].keys())[1:]
            else:
                fields = list(inst_description['fields'].keys())
            regfile_restrictions = inst_description['regfile_restrictions']
            operands = fields + ['none'] * (4 - len(fields))
            operands = [regfile_fillin(operands[i], regfile_restrictions) for i in range(len(operands))]
            majop = hex(int(inst_description['opcode'][:8], base=2))
            if len(inst_description['opcode']) == 8:
                minop = 0
            else:
                minop = hex(int(inst_description['opcode'][8:], base=2))
            if operands[0] == 'op':
                exop = 1
            else:
                exop = 0
            line = '   {{ "{}", {}, {}, {}, _OP({}), _OP({}), _OP({}), _OP({}), _T({})}},'\
                .format(inst_name, len(fields) - exop, majop, minop,
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

def gen_disassemble_file(insts: dict, output_file: str):
    with open(output_file, 'w') as f:
        print('#include <stdio.h>', file=f)
        print('\nenum dadao_disassemble_type {', file=f)
        dadao_disassemble_type_list = ['none','ha','hb','hc','hd','immu6','immu12','immu18','imms12','imms18','ww','immu16','offset12','offset18','offset24']
        for type in dadao_disassemble_type_list:
            type_string = '\t' + 'dadao_operand_' + type + ','
            print(type_string, file=f)
        print('};', file=f)
        print('\nstruct dadao_disassemble {', file=f)
        print('\tunsigned int opcode_mask;', file=f)
        print('\tunsigned int opcode_match;', file=f)
        print('\tenum dadao_disassemble_type op1;', file=f)
        print('\tenum dadao_disassemble_type op2;', file=f)
        print('\tenum dadao_disassemble_type op3;', file=f)
        print('\tenum dadao_disassemble_type op4;', file=f)
        print('\tint operand_num;', file=f)
        print('\tconst char *disassemble_format;', file=f)
        print('};\n', file=f)
        print('struct dadao_disassemble dadao_disassemble_opcodes[] = {',file=f)
        for inst_name, inst_description in insts.items():
            if inst_description['fields']['op'] == 8:
                fields = list(inst_description['fields'].keys())[1:]
                exop = 0
                operand_format = ['ha','hb','hc','hd']
                inst_opcode_mask = '0xff000000'
                inst_opcode_match = hex(int(inst_description['opcode'][:8], base=2)) + '000000'
            else:
                fields = list(inst_description['fields'].keys())
                exop = 1
                operand_format = ['hb','hc','hd','none']
                inst_opcode_mask = '0xfffc0000'
                inst_opcode_match = hex(int(inst_description['opcode'][:14] + '00', base=2)) + '0000'
            regfile_restrictions = inst_description['regfile_restrictions']
            operands = fields + ['none'] * (4 - len(fields))
            operands = [regfile_fillin(operands[i], regfile_restrictions) for i in range(len(operands))]
            if inst_name[0] == '_':
                inst_name = inst_name[1:]
            inst_disassemble_format = inst_name + '\t'
            first_operand = 1
            for i in range(exop,4):
                if operands[i] == 'none':
                    operand_format[i-exop] = operands[i]
                else:
                    if first_operand == 0:
                        inst_disassemble_format += ', '
                    first_operand = 0
                    if (operands[i] == 'imms24') | (inst_description['resources'].count('bpd') & (operands[i][0:3] == 'imm')):
                        operand_format[i-exop] = 'offset' + operands[i][4:]
                        operands[i] = 'offset'
                        inst_disassemble_format += ''
                    elif operands[i][0:4] == 'immu':
                        operand_format[i-exop] = operands[i]
                        inst_disassemble_format += '0x%x'
                    elif operands[i][0:4] == 'imms':
                        operand_format[i-exop] = operands[i]
                        inst_disassemble_format += '%d'
                    elif operands[i] == 'ww':
                        operand_format[i-exop] = operands[i]
                        inst_disassemble_format += 'w%d'
                    else:
                        inst_disassemble_format += operands[i] + '%d'
            line = '   {{ {}, {}, dadao_operand_{}, dadao_operand_{}, dadao_operand_{}, dadao_operand_{}, {}, "{}"}},'\
                .format(inst_opcode_mask, inst_opcode_match, operand_format[0], operand_format[1], operand_format[2], operand_format[3], len(fields) - exop, inst_disassemble_format)
            print(line, file=f)
        print(' };',file=f)

def gen_bitpat_file(insts: dict, output_file: str):
    with open(output_file, 'w') as f:
        for inst_name, inst_description in insts.items():
            bitpat = inst_description['opcode']
            if inst_description['fields']['op'] == 8:
                fields = list(inst_description['fields'].keys())[1:]
                bitpat += '????????????????????????'
            else:
                fields = list(inst_description['fields'].keys())
                bitpat += '??????????????????'
            regfile_restrictions = inst_description['regfile_restrictions']
            operands = fields + ['none'] * (4 - len(fields))
            operands = [regfile_fillin(operands[i], regfile_restrictions) for i in range(len(operands))]
            insts_temp = insts
            output_string = inst_name.upper()
            if inst_name[0] == '_':
                search_inst = inst_name[1:]
                output_string = output_string[1:]
            else:
                search_inst = '_' + inst_name
            if search_inst in insts_temp.keys():
                if insts_temp[inst_name]['optype'] == insts_temp[search_inst]['optype']:
                    for i in insts_temp[inst_name]['regfile_restrictions'].keys():
                        if insts_temp[inst_name]['regfile_restrictions'][i] != insts_temp[search_inst]['regfile_restrictions'][i]:
                            output_string += insts_temp[inst_name]['regfile_restrictions'][i]
                            break;
                else:
                    list1 = list(insts_temp[inst_name]['fields'].keys())
                    list2 = list(insts_temp[search_inst]['fields'].keys())
                    for i in range(0,4):
                        if list[i] == 'op':
                            continue;
                        if list1[i] != list2[i]:
                            if list1[i][0:3] == 'imm':
                                output_string += 'i'
                                break;
                            elif list2[i][0:3] == 'imm':
                                output_string += 'r'
                                break;
                            elif insts_temp[inst_name]['regfile_restrictions'][list1[i]] != insts_temp[search_inst]['regfile_restrictions'][list2[i]]:
                                output_string += insts_temp[inst_name]['regfile_restrictions'][list1[i]]
                                break;
            line = '   def {}\t\t= BitPat("b{}")'.format(output_string,bitpat)
            print(line, file=f)

def trans_dict(insts: dict):
    dasm_dict = {}
    for inst_name, inst_description in insts.items():
        if inst_description['fields']['op'] == 8:
            fields = list(inst_description['fields'].keys())[1:]
        else:
            fields = list(inst_description['fields'].keys())
        regfile_restrictions = inst_description['regfile_restrictions']
        if inst_name[0] == '_':
            inst_name = inst_name[1:]
        dasm_dict[inst_description['opcode']] = {
            'inst_name': inst_name,
            'fields': fields,
            'regfile': regfile_restrictions,
            'resources': inst_description['resources'],
        }
    return dasm_dict

def output_dasm(dasm_dict: dict, instruction: str):
    if len(instruction) != 8:
        print("wrong input format")
    else:
        output = ''
        instruction = bin(int(instruction,16))[2:].zfill(32)
        if instruction[0:8] == '00010000':
            key = instruction[0:14]
        else:
            key = instruction[0:8]
        if key not in dasm_dict:
            return 'unknown insn'
        output += dasm_dict[key]['inst_name'] + '\t'
        operands = dasm_dict[key]['fields']
        regfile = dasm_dict[key]['regfile']
        next_op = 0
        for operand in operands:
            if next_op == 1:
                output += ', '
            if operand == 'op':
                continue;
            next_op = 1
            if operand == 'ha':
                data = instruction[8:14]
                output += regfile['ha'] + str(int(data,2))
            elif operand == 'hb':
                data = instruction[14:20]
                output += regfile['hb'] + str(int(data,2))
            elif operand == 'hc':
                data = instruction[20:26]
                output += regfile['hc'] + str(int(data,2))
            elif operand == 'hd':
                data = instruction[26:32]
                output += regfile['hd'] + str(int(data,2))
            elif operand == 'ww':
                data = instruction[14:16]
                output += 'w' + str(int(data,2))
            elif operand[0:4] == 'immu':
                data = instruction[32-int(operand[4:]):32]
                output += str(hex(int(data,2)))
            elif operand[0:4] == 'imms':
                data = instruction[32-int(operand[4:]):32]
                if data[0] == '0':
                    imm = (int(data,2))
                else:
                    dec = int(data[1:],2) - 0x01
                    imm = -(~dec & int("0b" + "1" * (len(data) -1),2))
                if (operand == 'imms24') | (operand[0:4] == 'imms') & (dasm_dict[key]['resources'].count('bpd')):
                    imm = imm * 4
                    output += 'pc + ' + str(imm)
                else:
                    output += str(imm)
            elif operand == 'none':
                next_op = 0
                break;
        return output

def main():
    input_decode = ''
    output_decode = ''
    output_opc = ''
    output_encoding = ''
    output_disassemble = ''
    output_bitpat = ''
    dasm = 0
    if (len(sys.argv) == 1) | (sys.argv.count('--help')):
        help_information()
        return
    long_opts = ['input=','decode=','opc=','encoding=','disassemble=','bitpat=','dasm=']
    try:
        (opts, args) = getopt.gnu_getopt(sys.argv[1:], 'o:vw:', long_opts)
    except getopt.GetoptError as err:
        print('get opts or args error')
    for o, a in opts:
        if o == '--input':
            input_decode = a
        elif o == '--decode':
            output_decode = a
        elif o == '--opc':
            output_opc = a
        elif o == '--encoding':
            output_encoding = a
        elif o == '--disassemble':
            output_disassemble = a
        elif o == '--bitpat':
            output_bitpat = a
        elif o == '--dasm':
            if a == 'dadao':
                dasm = 1
    if input_decode:
        insts = read_opcodes(input_decode)
    if output_decode:
        gen_decode_file(insts, output_decode)
    if output_opc:
        gen_opc_file(insts, output_opc)
    if output_encoding:
        gen_encoding_file(insts, output_encoding)
    if output_disassemble:
        gen_disassemble_file(insts, output_disassemble)
    if output_bitpat:
        gen_bitpat_file(insts, output_bitpat)
    while dasm:
        try:
            line = input()
        except EOFError:
            break;
        if line == '':
            break;
        if line.find('DASM') >= 0:
            instruction = line[line.find('DASM'):]
            instruction = instruction[5:13]
            dasm_dict = trans_dict(insts)
            trans_line = line[0:line.find('DASM')] + output_dasm(dasm_dict,instruction)
            print(trans_line)
        else:
            print(line)

if __name__ == '__main__':
    import io
    import os
    import re
    import sys
    import getopt
    main()
