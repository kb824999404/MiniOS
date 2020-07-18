/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            user.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Kaibin Zhou, 2020
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef	_MINIOS_USER_H_
#define	_MINIOS_USER_H_


#define MAX_USER_COUNT 10				   //最大用户数量
struct Users{
	char username[128];
	char password[128];
};
extern struct Users users[MAX_USER_COUNT]; //用户
extern char currentUser[128];              //当前用户名
extern int usercount;					   //用户数量
extern int currentUserIndex;			   //当前用户名索引

#endif /* _MINIOS_USER_H_ */