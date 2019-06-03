#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion statrting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
void sched_yield(void)
{
        static int select=0;
        static int time_count=0;
        static struct Env *pe=NULL;
        static int i=0;
        static int changed =0;
        if(changed){
                changed=0;
                pe=NULL;
        }
        while(pe==NULL||pe->env_status!=ENV_RUNNABLE){
                LIST_FOREACH(pe,&env_sched_list[select],env_sched_link){
                        if(pe->env_status==ENV_RUNNABLE&&pe->env_pri>0){
                                time_count=pe->env_pri;
                                break;
                        }
                }
                if(pe!=NULL){
                        break;
                }
                select=(select+1)%2;
        }
        time_count--;
        if(time_count<=0){
                LIST_REMOVE(pe,env_sched_link);
                LIST_INSERT_HEAD(&env_sched_list[(select+1)%2],pe,env_sched_link);
                changed=1;
        }
        env_run(pe);
}

