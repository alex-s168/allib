#include "../smallstr/smallstr.h"

int main() {
    smallstr str = smallstr_from_trunc("Hello, , world!");
    smallstr b = smallstr_substr(&str, opstr_find(smallstr_op(&str), ','), -1);
    return strcmp(str.str, ", , world!");
}
