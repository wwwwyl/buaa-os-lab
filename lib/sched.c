#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.15 ***/
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    struct Env *nextenv = curenv;
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
	if(curenv == NULL || count <= 0 || curenv->env_status != ENV_RUNNABLE){
		if(curenv){
			LIST_INSERT_TAIL(&env_sched_list[!point], curenv, env_sched_link);
		}
		int findenv = 0;
		while(1){
			if (LIST_EMPTY(&env_sched_list[point])){
				point = !point;
				break;
			}
			nextenv = LIST_FIRST(&env_sched_list[point]);
			if (nextenv->env_status == ENV_RUNNABLE){findenv=1;break;}
			LIST_REMOVE(nextenv, env_sched_link);
			LIST_INSERT_TAIL(&env_sched_list[!point], nextenv, env_sched_link);
		}
		if(!findenv){
			while(1){
                        	if (LIST_EMPTY(&env_sched_list[point])){
                        		panic("^^^^^^No env is RUNNABLE!^^^^^^");
				}
                        	nextenv = LIST_FIRST(&env_sched_list[point]);
                        	if (nextenv->env_status == ENV_RUNNABLE){findenv=1;break;}
                        	LIST_REMOVE(nextenv, env_sched_link);
                        	LIST_INSERT_TAIL(&env_sched_list[!point], nextenv, env_sched_link);
                	}
		}
		LIST_REMOVE(nextenv, env_sched_link);
		count = nextenv->env_pri - 1;
		env_run(nextenv);
		panic("^^^^^^sched yield jump faild^^^^^^");
	}
	count--;
	env_run(curenv);
	panic("^^^^^^sched yield reached end^^^^^^");
}
