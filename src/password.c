#include <string.h>
#include "password.h"


#ifdef __cplusplus
extern "C" {
#endif


enum PASSWORD_STATE {
    IDLE,
    PROCESSING_KEY,
};


struct __password_state {
    enum PASSWORD_STATE state;
    char const *key;
    char input_buffer[8];
    unsigned long key_length;
    unsigned long count;

} password_state = {
    IDLE,
    0,
    {0},
    0,
};

void password_set_key(char const *key)
{
    unsigned long length;

    password_state.key = key;
    password_state.key_length = 0;

    if (key) {
        length = strlen(key);
        password_state.key_length = (length < 8) ? length : 8;
    }
}


enum PASSWORD_RESULT password_process_key(char key)
{
    switch (password_state.state) {
        case IDLE:
            password_state.count = 0;
            password_state.state = PROCESSING_KEY;

        case PROCESSING_KEY:
            if (!password_state.key) {
                password_state.state = IDLE;
                return CORRECT_PASSWORD;
            }

            password_state.input_buffer[password_state.count] = key;
            password_state.count++;

            if (password_state.count == password_state.key_length) {
                password_state.state = IDLE;
                if (!memcmp(password_state.input_buffer, password_state.key, password_state.key_length)) {
                    return CORRECT_PASSWORD;
                } else {
                    return WRONG_PASSWORD;
                }
            }
    }

    return NONE;
}

#ifdef __cplusplus
}
#endif

