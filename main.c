    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
    #define HASHSIZE 128

    typedef struct	t_list
    {
        char *def_name;
        char *value;
        struct t_list *next;

    }		list;

    list	*lst_new(char *def_name, char *value)
    {
        list *new;

        new = (list*)malloc(sizeof(list));
        new->next = NULL;
        new->def_name = (char*)malloc(sizeof(char) * (strlen(def_name) + 1));
        strcpy(new->def_name, def_name);
        new->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(new->value, value);
        return (new);
    }

    char	*search(list *lst, char *str)
    {
        while (lst && strcmp(lst->def_name, str))
            lst = lst->next;
        if (lst)
            return(lst->value);
        return (str);
    }

    void	push_front(list **lst, char *value, char *def_name)
    {
        list *new;

        new = lst_new(def_name, value);
        new->next = *lst;
        *lst = new;
    }

    int symbol_to_int(char c)
    {
        if (isdigit(c))
            return (c - '0');
        if (c >= 'A' && c <= 'Z')
            return (10 + c - 'A');
        if (c >= 'a' && c <= 'z')
            return (36 + c - 'A');
    }

    int	hash_sum(char *data)
    {
        int result = 0;

        while (*data)
        {
            result = result * 62 + symbol_to_int(*data);
            result %= HASHSIZE;
            data++;
        }
        return (result);
    }

    void	check(char *str, list *array[HASHSIZE], FILE *f, char symbol, FILE *f_result)
    {
        char c;
        char def_name[30];
        char value[30];
        char *ptr_value = value;
        char *ptr_def = def_name;
        int hash;

        if (!strcmp(str, "#define"))
        {
            if (symbol != EOF)
                fprintf(f_result, "%s%c", str, symbol);
            else
                fprintf(f_result, "%s", str);
            while ((c = fgetc(f)))
            {
                fprintf(f_result, "%c", c);
                if (c == ' ')
                {
                    *ptr_def = 0;
                    break;
                }
                *ptr_def++ = c;
            }
            while ((c = fgetc(f)))
            {
                fprintf(f_result, "%c", c);
                if (c == '\n' || c == EOF)
                {
                    *ptr_value = 0;
                    break;
                }
                *ptr_value++ = c;
            }
            hash = hash_sum(def_name);
            push_front(&array[hash], value, def_name);
            return ;
        }
        hash = hash_sum(str);
        if (symbol != EOF)
            fprintf(f_result, "%s%c", search(array[hash], str), symbol);
        else
            fprintf(f_result, "%s", search(array[hash], str));
    }

    void	print_error(int id, char *name)
    {
        exit(id);
    }

    void	parser(list *array[HASHSIZE], FILE *f)
    {
        char c;
        char word[30];
        char *ptr = word;
        FILE *f_result;

        if (!(f_result = fopen("result.txt", "w")))
            print_error(0, "result.txt");
        while ((c = fgetc(f)))
        {
            if (c == ' ' || c == '\n' || c == EOF)
            {
                *ptr = 0;
                check(word, array, f, c, f_result);
                if (c == EOF)
                    break ;
                ptr = word;
                continue ;
            }
            *ptr++ = c;
        }

    }

    void clean(list *lst)
    {
        list *temp = lst;

        while (lst)
        {
            temp = lst;
            lst = lst->next;
            free(temp);
        }
    }

    void del_all(list *array[HASHSIZE])
    {
        for (int i = 0; i < HASHSIZE; i++)
        {
            clean(array[i]);
        }

    }

    int main(int argc, char **argv)
    {
        list *array[HASHSIZE] = {NULL};
        FILE *f;
        if (!(f = fopen(argv[1], "r")))
            print_error(0, argv[1]);
        parser(array, f);
        del_all(array);
    }
