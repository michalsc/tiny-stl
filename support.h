#ifndef _SUPPORT_H
#define _SUPPORT_H

#define MEMF_ANY    1
#define MEMF_CLEAR  2

void * AllocMem(int size, int type);
void FreeMem(void *ptr, int size);
void CopyMem(const void *src, void *dst, int size);
void SetMem(void *dst, int size, char fill);
int _strcmp_(const char *s1, const char *s2);
int _strlen_(const char *s1);

#endif // _SUPPORT_H
