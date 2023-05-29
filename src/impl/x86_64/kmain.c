void kmain(void)
{
    const char *str = "my first kernel";
    char *vidptr = (char *)0xb8000; // video mem begins here.

    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }

    vidptr[0] = 'H';
    vidptr += 2;
    vidptr[0] = 'e';
    vidptr += 2;
    vidptr[0] = 'l';

    return;
}