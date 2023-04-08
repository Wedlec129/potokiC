#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[]){


	char *args[2]={"PROGRAM",NULL};
	int pipefd[2];
	int pipefd2[2];

	if (argc !=4){
	printf("Usage: %s <prog1> <prog2> <prog3> \n", argv[0]);
	return __LINE__;
	}

	if (0!=pipe(pipefd)){
	perror("pipe");
	return __LINE__;
	}

	switch (fork()){
		case -1:
			perror("fork");
			return __LINE__;

		//дочерний	
		case 0:

			switch (fork()){

				case 0:
					close(pipefd2[1]);
					close(1);
					dup2(pipefd2[0],0);
					args[0]=argv[3];
					execve(args[0],args,envp);
					break;

				default:
					close(pipefd[1]);
					close(1);
					dup2(pipefd2[0],1);
					args[0]=argv[2];
					execve(args[0],args,envp);
					break;

			}
		//род
		default:
				close(pipefd[0]);
				close(1);
				dup2(pipefd[1],1);
				args[0]=argv[1];
				execve(args[0],args,envp);
				break;
	}


perror("execve");
return __LINE__;


}



