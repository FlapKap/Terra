#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "operators.h"

Env *init_env(void);
void clear_env(Env *env);
Number get_value(Env *env, int index);
void set_value(Env *env, int index, Number val);
void clear_stack(Env *env);
Stack *get_stack(Env *env);
void resize_env(Env *env);

#endif /* ENVIRONMENT_H */