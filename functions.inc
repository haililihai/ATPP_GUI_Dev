
#--------------------------Initilization----------------------------------#
PIPELINE=$(pwd)
WD=''
DATA_DIR=''
SUB_LIST=''
ROI=''
MAX_CL_NUM=2
POOLSIZE=7
stopid=''
logfile=''

switch=()
LH=0
RH=0
GE=0
split_half=0
pairwise=0
leave_one_out=0
hi_vi=0
tpd=0
cont=0
silhouette=0

gtk "gtk_entry_set_text ${dir_work} \"/DATA/233/hli/test_STN_parcellation\""
gtk "gtk_entry_set_text ${dir_data} \"/DATA/233/hli/Data/chengdu\""
gtk "gtk_entry_set_text ${file_sub} \"/DATA/233/hli/Data/chengdu/sub_CD_2.txt\""
gtk "gtk_entry_set_text ${name_ROI} \"STN\""


gtk "gtk_entry_set_text ${exe_matlab} \"$(command -v matlab)\""
gtk "gtk_entry_set_text ${exe_qsub} \"$(command -v qsub)\""
gtk "gtk_entry_set_text ${exe_fsl_sub} \"$(command -v fsl_sub)\""
gtk "gtk_entry_set_text ${exe_probtrackx} \"$(command -v probtrackx)\""
gtk "gtk_entry_set_text ${dir_pl} \"$(pwd)\""
gtk "gtk_entry_set_text ${dir_spm} \"/DATA/233/hli/toolbox/spm8\""
gtk "gtk_entry_set_text ${dir_nifti} \"$(pwd)/NIfTI_20130306\""
define spm_entry gtk "gtk_entry_get_text ${dir_spm}"
gtk "gtk_entry_set_text ${file_temp} \"$(pwd)/MNI152_T1_1mm_brain.nii\""
gtk "gtk_entry_set_text ${name_n_samples} \"5000\""
gtk "gtk_entry_set_text ${name_dis_cor} \"1\""
gtk "gtk_entry_set_text ${name_len_step} \"0.5\""
gtk "gtk_entry_set_text ${name_n_steps} \"2000\""
gtk "gtk_entry_set_text ${name_cur_thres} \"0.2\""
gtk "gtk_entry_set_text ${name_val_thres} \"10\""
gtk "gtk_entry_set_text ${name_down_size} \"5\""
gtk "gtk_entry_set_text ${name_method} \"sc\""
gtk "gtk_entry_set_text ${name_vox_size} \"1\""
gtk "gtk_entry_set_text ${name_group_thres} \"0.25\""
gtk "gtk_entry_set_text ${name_mpm_thres} \"0.25\""
gtk "gtk_entry_set_text ${name_n_iter} \"100\""

define status_ready gtk "gtk_statusbar_get_context_id ${statusbar} \"ready\""
gtk "gtk_statusbar_push ${statusbar} ${status_ready} \"Ready\""

#gtk "gtk_statusbar_pop ${statusbar} ${status_ready}"


#-------------------------Callback functions------------------------------#
function on_button_dir_pl_file_set()
{
	define dir_pl_entry gtk "gtk_file_chooser_get_uri ${button_dir_pl}"
	gtk "gtk_entry_set_text ${dir_pl} \"${dir_pl_entry##file://}\""
}

function on_button_dir_data_file_set()
{
	define dir_data_entry gtk "gtk_file_chooser_get_uri ${button_dir_data}"
	gtk "gtk_entry_set_text ${dir_data} \"${dir_data_entry##file://}\""
}

function on_button_dir_work_file_set()
{
	define dir_work_entry gtk "gtk_file_chooser_get_uri ${button_dir_work}"
	gtk "gtk_entry_set_text ${dir_work} \"${dir_work_entry##file://}\""
}

function on_button_file_sub_file_set()
{
	define file_sub_entry gtk "gtk_file_chooser_get_filename ${button_file_sub}"
	gtk "gtk_entry_set_text ${file_sub} \"${file_sub_entry}\""
}

function on_button_dir_spm_file_set()
{
	define dir_spm_entry gtk "gtk_file_chooser_get_uri ${button_dir_spm}"
	gtk "gtk_entry_set_text ${dir_spm} \"${dir_spm_entry##file://}\""
}

function on_button_dir_nifti_file_set()
{
	define dir_nifti_entry gtk "gtk_file_chooser_get_uri ${button_dir_nifti}"
	gtk "gtk_entry_set_text ${dir_nifti} \"${dir_nifti_entry##file://}\""
}

function on_button_file_temp_file_set()
{
	define file_temp_entry gtk "gtk_file_chooser_get_filename ${button_file_temp}"
	gtk "gtk_entry_set_text ${file_temp} \"${file_temp_entry}\""
}

function on_button_exe_matlab_file_set()
{
	define exe_matlab_entry gtk "gtk_file_chooser_get_filename ${button_exe_matlab}"
	gtk "gtk_entry_set_text ${exe_matlab} \"${exe_matlab_entry}\""
}

function on_button_exe_qsub_file_set()
{
	define exe_qsub_entry gtk "gtk_file_chooser_get_filename ${button_exe_qsub}"
	gtk "gtk_entry_set_text ${exe_qsub} \"${exe_qsub_entry}\""
}
function on_button_exe_fsl_sub_file_set()
{
	define exe_fsl_sub_entry gtk "gtk_file_chooser_get_filename ${button_exe_fsl_sub}"
	gtk "gtk_entry_set_text ${exe_fsl_sub} \"${exe_fsl_sub_entry}\""
}
function on_button_exe_probtrackx_file_set()
{
	define exe_probtrackx_entry gtk "gtk_file_chooser_get_filename ${button_exe_probtrackx}"
	gtk "gtk_entry_set_text ${exe_probtrackx} \"${exe_probtrackx_entry}\""
}

function on_GE_toggled()
{
	if [[ ${GE} == 0 ]]
	then
		GE=1
	else 
		GE=0
	fi
}

function on_left_toggled()
{
	if [[ ${LH} == 0 ]]
	then
		LH=1
	else 
		LH=0
	fi
}

function on_right_toggled()
{
	if [[ ${RH} == 0 ]]
	then
		RH=1
	else 
		RH=0
	fi
}

function on_step0_toggled()
{
	if [[ ${switch[@]/%s0/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s0/})
	else
	 	switch+=(s0)
	fi
}

function on_step1_toggled()
{
	if [[ ${switch[@]/%s1/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s1/})
	else
	 	switch+=(s1)
	fi
}

function on_step2_toggled()
{
	if [[ ${switch[@]/%s2/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s2/})
	else
	 	switch+=(s2)
	fi
}

function on_step3_toggled()
{
	if [[ ${switch[@]/%s3/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s3/})
	else
	 	switch+=(s3)
	fi
}

function on_step4_toggled()
{
	if [[ ${switch[@]/%s4/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s4/})
	else
	 	switch+=(s4)
	fi
}

function on_step5_toggled()
{
	if [[ ${switch[@]/%s5/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s5/})
	else
	 	switch+=(s5)
	fi
}

function on_step6_toggled()
{
	if [[ ${switch[@]/%s6/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s6/})
	else
	 	switch+=(s6)
	fi
}

function on_step7_toggled()
{
	if [[ ${switch[@]/%s7/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s7/})
	else
	 	switch+=(s7)
	fi
}

function on_step8_toggled()
{
	if [[ ${switch[@]/%s8/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s8/})
	else
	 	switch+=(s8)
	fi
}

function on_step9_toggled()
{
	if [[ ${switch[@]/%s9/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s9/})
	else
	 	switch+=(s9)
	fi
}

function on_step10_toggled()
{
	if [[ ${switch[@]/%s10/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s10/})
	else
	 	switch+=(s10)
	fi
}

function on_step11_toggled()
{
	if [[ ${switch[@]/%s11/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s11/})
	else
	 	switch+=(s11)
	fi
}

function on_step12_toggled()
{
	if [[ ${switch[@]/%s12/} != ${switch[@]} ]]
	then
		switch=(${switch[@]/%s12/})
	else
	 	switch+=(s12)
	fi
}

function on_split_half_toggled()
{
	if [[ ${split_half} == 0 ]]
	then
		split_half=1
	else 
		split_half=0
	fi
}

function on_pairwise_toggled()
{
	if [[ ${pairwise} == 0 ]]
	then
		pairwise=1
	else 
		pairwise=0
	fi
}

function on_leave_one_out_toggled()
{
	if [[ ${leave_one_out} == 0 ]]
	then
		leave_one_out=1
	else 
		leave_one_out=0
	fi
}

function on_hi_vi_toggled()
{
	if [[ ${hi_vi} == 0 ]]
	then
		hi_vi=1
	else 
		hi_vi=0
	fi
}

function on_tpd_toggled()
{
	if [[ ${tpd} == 0 ]]
	then
		tpd=1
	else 
		tpd=0
	fi
}

function on_silhouette_toggled()
{
	if [[ ${silhouette} == 0 ]]
	then
		silhouette=1
	else 
		silhouette=0
	fi
}

function on_cont_toggled()
{
	if [[ ${cont} == 0 ]]
	then
		cont=1
	else 
		cont=0
	fi
}


function on_button_run_clicked()
{	
	config_file=$(pwd)/log/config_$(date +%m-%d_%H-%M-%S).sh
	touch ${config_file}
	
	define res_dir_pl gtk "gtk_entry_get_text ${dir_pl}"
	echo "PIPELINE=${res_dir_pl}" >> ${config_file}
	PIPELINE=${res_dir_pl}

	define res_dir_work gtk "gtk_entry_get_text ${dir_work}"
	echo "WD=${res_dir_work}" >> ${config_file}
	WD=${res_dir_work}

	define res_dir_data gtk "gtk_entry_get_text ${dir_data}"
	echo "DATA_DIR=${res_dir_data}" >> ${config_file}

	define res_file_sub gtk "gtk_entry_get_text ${file_sub}"
	echo "SUB_LIST=${res_file_sub}" >> ${config_file}

	define res_ROI gtk "gtk_entry_get_text ${name_ROI}"
	echo "ROI=${res_ROI}" >> ${config_file}
	ROI=${res_ROI}

	define res_max_cl gtk "gtk_spin_button_get_value_as_int ${spin_max_cl}"
	echo "MAX_CL_NUM=${res_max_cl}" >> ${config_file}

	define res_dir_spm gtk "gtk_entry_get_text ${dir_spm}"
	echo "SPM=${res_dir_spm}" >> ${config_file}

	define res_dir_nifti gtk "gtk_entry_get_text ${dir_nifti}"
	echo "NIFTI=${res_dir_nifti}" >> ${config_file}

	define res_file_temp gtk "gtk_entry_get_text ${file_temp}"
	echo "TEMPLATE=${res_file_temp}" >> ${config_file}

	define res_exe_matlab gtk "gtk_entry_get_text ${exe_matlab}"
	echo "COMMAND_MATLAB=${res_exe_matlab}" >> ${config_file}

	define res_exe_qsub gtk "gtk_entry_get_text ${exe_qsub}"
	echo "COMMAND_QSUB=${res_exe_qsub}" >> ${config_file}

	define res_exe_fsl_sub gtk "gtk_entry_get_text ${exe_fsl_sub}"
	echo "COMMAND_FSLSUB=${res_exe_fsl_sub}" >> ${config_file}

	define res_exe_probtrackx gtk "gtk_entry_get_text ${exe_probtrackx}"
	echo "COMMAND_PROBTRACKX=${res_exe_probtrackx}" >> ${config_file}

	define res_poolsize gtk "gtk_spin_button_get_value_as_int ${spin_poolsize}"
	echo "POOLSIZE=${res_poolsize}" >> ${config_file}

	echo "GE=${GE}" >> ${config_file}

	echo "LEFT=${LH}" >> ${config_file}
	echo "RIGHT=${RH}" >> ${config_file}

	echo "split_half=${split_half}" >> ${config_file}
	echo "pairwise=${pairwise}" >> ${config_file}
	echo "leave_one_out=${leave_one_out}" >> ${config_file}
	echo "hi_vi=${hi_vi}" >> ${config_file}
	echo "tpd=${tpd}" >> ${config_file}
	echo "cont=${cont}" >> ${config_file}
	echo "silhouette=${silhouette}" >> ${config_file}
	
	echo "SWITCH=(${switch[@]})" >> ${config_file}

	define res_n_samples gtk "gtk_entry_get_text ${name_n_samples}"
	echo "N_SAMPLES=${res_n_samples}" >> ${config_file}

	define res_dis_cor gtk "gtk_entry_get_text ${name_dis_cor}"
	echo "DIS_COR=${res_dis_cor}" >> ${config_file}

	define res_len_step gtk "gtk_entry_get_text ${name_len_step}"
	echo "LEN_STEP=${res_len_step}" >> ${config_file}

	define res_n_steps gtk "gtk_entry_get_text ${name_n_steps}"
	echo "N_STEPS=${res_n_steps}" >> ${config_file}

	define res_cur_thres gtk "gtk_entry_get_text ${name_cur_thres}"
	echo "CUR_THRES=${res_cur_thres}" >> ${config_file}

	define res_val_thres gtk "gtk_entry_get_text ${name_val_thres}"
	echo "VAL_THRES=${res_val_thres}" >> ${config_file}
	
	define res_down_size gtk "gtk_entry_get_text ${name_down_size}"
	echo "DOWN_SIZE=${res_down_size}" >> ${config_file}
	
	define res_method gtk "gtk_entry_get_text ${name_method}"
	echo "METHOD=${res_method}" >> ${config_file}

	define res_vox_size gtk "gtk_entry_get_text ${name_vox_size}"
	echo "VOX_SIZE=${res_vox_size}" >> ${config_file}

	define res_group_thres gtk "gtk_entry_get_text ${name_group_thres}"
	echo "GROUP_THRES=${res_group_thres}" >> ${config_file}
	
	define res_mpm_thres gtk "gtk_entry_get_text ${name_mpm_thres}"
	echo "MPM_THRES=${res_mpm_thres}" >> ${config_file}

	define res_n_iter gtk "gtk_entry_get_text ${name_n_iter}"
	echo "N_ITER=${res_n_iter}" >> ${config_file}

	runrun=$(bash ${PIPELINE}/ATPP.sh ${config_file})
	stopid=$(echo ${runrun}|awk '{print $1}')
	echo "stopid=${stopid}" >> ${config_file}
	logfile=$(echo ${runrun}|awk '{print $2}')
	echo "logfile=${logfile}" >> ${config_file}

	define status_run gtk "gtk_statusbar_get_context_id ${statusbar} \"run\""
	gtk "gtk_statusbar_push ${statusbar} ${status_run} \"Running...\""
	
}

function on_button_edit_clicked()
{
	gedit $(pwd)/0_gen_WD.sh
}

function getcpid() {
    cpids=`pgrep -P $1|xargs`
    for cpid in $cpids;
    do
        echo "$cpid"
        getcpid $cpid
    done
}

function on_button_stop_clicked()
{
	source ${config_file}
	if [ "${GE}" == "1" ]; then
		qdel ${stopid}
	else
		kill $(getcpid ${stopid})
	fi
	define status_stop gtk "gtk_statusbar_get_context_id ${statusbar} \"stop\""
	gtk "gtk_statusbar_push ${statusbar} ${status_stop} \"Stop!\""
}


function on_button_log_clicked()
{
	source ${config_file}
	gedit ${logfile}
}

function on_button_progress_clicked()
{
	source ${config_file}
	gedit ${WD}/log/progress_check.txt
}