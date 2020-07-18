/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            user.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Kaibin Zhou, 2020
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef	_MINIOS_FS_H_
#define	_MINIOS_FS_H_

extern char currentFolder[128];
extern char filepath[128];
extern char files[20][128];
extern char userfiles[20][128];
extern int filequeue[50];
extern int filecount;
extern int isEntered;
extern int leiflag;

#endif /* _MINIOS_FS_H_ */