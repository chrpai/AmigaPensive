/*
    Protos for XPR
*/
int xprinit(char*);
int xprsend(void);
int xprreceive(void);
void xprclose(void);

void __stdargs plog(char,char*,long arg1,long arg2);
