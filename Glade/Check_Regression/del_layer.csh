#!/bin/csh

set job = $1
set step = $2
set layer = $3

COM open_job,job=$job
COM open_entity,job=$job,type=step,name=$step,iconic=no
COM check_inout,job=$job,mode=out,ent_type=job
COM delete_layer,layer=cr_${layer}_tmp
COM save_job,job=${job},override=no,skip_upgrade=no
COM check_inout,job=${job} ,mode=in,ent_type=job
COM close_job,job=${job}
exit(0)

