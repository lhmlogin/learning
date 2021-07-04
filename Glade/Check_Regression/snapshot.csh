#!/bin/csh

set job = $1
set step = $2
set chklst = $3
set layer = $4
set note_num = (`echo $5|sed 's/,/ /g'`)
set note_layer = $6
set snapshot_path = "/workfile/maintain_scripts/check_regression_version/snapshot"
set cur_version = $7
set compare_version = $8
set picture_path = $snapshot_path/$job/$compare_version

COM open_job,job=$job
COM open_entity,job=$job,type=step,name=$step,iconic=no
COM units,type=mm

COM chklist_open,chklist=$chklst
COM chklist_show,chklist=$chklst
COM check_inout,job=$job,mode=out,ent_type=job
COM chklist_run,chklist=$chklst,nact=a,area=global

mkdir -p $picture_path

foreach i ($note_num)
	COM info, out_file=/tmp/info-incam186ad.$$, write_mode=replace,units=mm,args=  -t notes -e ${job}/${step}/${note_layer}/notes -m script -d NOTE -p x+y -s $i -o consider_origin
	source /tmp/info-incam186ad.$$
	COM display_layer,name=$layer,display=yes
	COM work_layer,name=$layer
	COM display_layer,name=cr_${layer}_tmp,display=yes,number=1
	COM display_layer,name=${layer},display=yes,number=2
	COM pan_center,x=$gNOTEx,y=$gNOTEy
	set x1 = `echo $gNOTEx + 1 |bc -l`
	set x2 = `echo $gNOTEx - 1 |bc -l`
	set y1 = `echo $gNOTEy + 1 |bc -l`
	set y2 = `echo $gNOTEy - 1 |bc -l`
	COM zoom_area,x1=$x1,y1=$y1,x2=$x2,y2=$y2
	set note = "snapshot of note${i}! Red is the result of $cur_version, the other is the result of $compare_version"
	COM save_snapshot,path=${picture_path}/${i}.png, note1=$note
end

COM check_inout,job=${job},mode=in,ent_type=job 
COM close_job,job=${job}
COM close_toolkit,save_log=no
exit(0)


