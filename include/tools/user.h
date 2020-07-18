#ifndef	_MINIOS_USER_H_
#define	_MINIOS_USER_H_


#define MAX_USER_COUNT 10
struct Users{
	char username[128];
	char password[128];
};
extern struct Users users[MAX_USER_COUNT];
extern char currentUser[128];
extern int usercount;
extern int currentUserIndex;

int vertify();
void initData();
void login();
void showUsers();
void addUser(char *username, char *password);
void updateUsers();

#endif /* _MINIOS_USER_H_ */