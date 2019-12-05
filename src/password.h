#ifndef _PASSWORD_H
#define _PASSWORD_H

#ifdef __cplusplus
extern "C" {
#endif


enum PASSWORD_RESULT {
    NONE,
    CORRECT_PASSWORD,
    WRONG_PASSWORD,
};



enum PASSWORD_RESULT password_process_key(char key);
void password_set_key(char const *key);

#ifdef __cplusplus
}
#endif

#endif
