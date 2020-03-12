#include <stdio.h>
#include "card.h"

int main(int argc, char const *argv[])
{
    Card *c = Card__new("J", "hearts");
    printf("%s", Card__print(c));
    return 0;
}
