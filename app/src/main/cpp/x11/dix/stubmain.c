int dix_main(int argc, char *argv[], char *envp[]);

/*
  A default implementation of main, which can be overridden by the DDX
 */
int
main_stubmain(int argc, char *argv[], char *envp[])
{
    return dix_main(argc, argv, envp);
}
