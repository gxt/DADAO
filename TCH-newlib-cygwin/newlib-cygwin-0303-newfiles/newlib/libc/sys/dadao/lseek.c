
int lseek(int file, int ptr, int dir) {
  return 0;
}

int _lseek(int file, int ptr, int dir) {
  return lseek(file, ptr, dir);
}
