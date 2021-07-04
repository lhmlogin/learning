#!/bin/csh


set job = $1
set step = $2
set chklst = $3
set layer = $4


COM open_job,job=$job
COM open_entity,job=$job,type=step,name=$step,iconic=no
COM affected_layer,mode=all,affected=no
COM affected_layer,name=$layer,mode=single,affected=yes
COM chklist_open,chklist=$chklst
COM chklist_show,chklist=$chklst
COM check_inout,job=$job,mode=out,ent_type=job
COM chklist_run,chklist=$chklst,nact=a,area=global
COM copy_layer,source_job=${job},source_step=${step},source_layer=${layer},dest=layer_name,dest_step=,dest_layer=cr_${layer}_tmp,mode=replace,invert=no,copy_notes=no,copy_attrs=new_layers_only
COM copy_layer,source_job=${job},source_step=${step},source_layer=${layer}+++,dest=layer_name,dest_step=,dest_layer=${layer},mode=replace,invert=no,copy_notes=no,copy_attrs=new_layers_only
COM save_job,job=${job},override=no,skip_upgrade=no
COM check_inout,job=${job},mode=in,ent_type=job
COM close_job,job=${job}
exit(0)



