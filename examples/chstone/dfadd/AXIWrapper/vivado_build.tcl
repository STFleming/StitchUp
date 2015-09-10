#Tcl script that builds a Zynq project using Vivado
#AXI SoC interconnect
#


set current_dir [pwd]
set fpga_part "xc7z020clg484-1"
set temp_prj_name "temp"
set board_name "xilinx.com:zynq:zc702:1.0"
set synth_language "VHDL"
set sim_language "VHDL"
set bd_name "zynq_sys"
set stitchup_ip_dir "./AXIWrapperStitchUp_1.0/"
set configMonkey "./sem_axi_1_1.0/"
set full_ip_name "imperial:user:AXIWrapperStitchUp:1.0"
set ip_instantiation_name "AXIWrapperStitchUp_0"
set configMonkeyFullName "imperial:stitchup:sem_axi_1:1.0"
set configMonkeyInstantName "sem_axi_1_0"

create_project $temp_prj_name $current_dir/tmp/$temp_prj_name -part $fpga_part -force
set_property board $board_name [current_project]
set_property target_language $synth_language [current_project]
set_property simulator_language $sim_language [current_project]

#Create the block design now that the project is created

create_bd_design "${bd_name}"

#Add the processing system
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.3 processing_system7_0
endgroup
#run block automation on the processing system
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" }  [get_bd_cells processing_system7_0]

#Add the output directory where the GOS IP core has been created
set_property ip_repo_paths ./ [current_fileset]
update_ip_catalog

#Add our newly generated IP block
startgroup
create_bd_cell -type ip -vlnv $full_ip_name $ip_instantiation_name
endgroup

#Add our newly generated IP block
startgroup
create_bd_cell -type ip -vlnv $configMonkeyFullName $configMonkeyInstantName
endgroup

#Connect the IP core to the PS via AXI using the names defined in Component.xml in the generated IP core
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_0/M_AXI_GP0" }  [get_bd_intf_pins $ip_instantiation_name/S00_AXI]
set_property offset 0x43C00000 [get_bd_addr_segs {processing_system7_0/Data/SEG_AXIWrapperStitchUp_0_S00_AXI_reg}]

#Connect the IP core to the PS via AXI using the names defined in Component.xml in the generated IP core
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_0/M_AXI_GP0" }  [get_bd_intf_pins $configMonkeyInstantName/S00_AXI]
set_property offset 0x42C00000 [get_bd_addr_segs {processing_system7_0/Data/SEG_sem_axi_1_0_S00_AXI_reg}]

#Save the current board design
save_bd_design

#Create System Wrapper
make_wrapper -files [get_files  $current_dir/tmp/$temp_prj_name/$temp_prj_name.srcs/sources_1/bd/$bd_name/${bd_name}.bd] -top
add_files -norecurse $current_dir/tmp/$temp_prj_name/$temp_prj_name.srcs/sources_1/bd/$bd_name/hdl/${bd_name}_wrapper.vhd
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

#set_property bitstream.seu.essentialbits yes [current_design]
set_property STEPS.WRITE_BITSTREAM.TCL.PRE /home/sf306/ExperimentsStitchUp/SEUInjection/chstone/dfadd/AXIWrapper/pre_tcl_file.tcl [get_runs impl_1]

#Build the bitstream
reset_run synth_1
reset_run impl_1
launch_runs synth_1
wait_on_run synth_1
launch_runs impl_1
wait_on_run impl_1
launch_runs impl_1 -to_step write_bitstream
wait_on_run impl_1


#Cleaning up
file copy -force $current_dir/tmp/$temp_prj_name/$temp_prj_name.runs/impl_1/${bd_name}_wrapper.bit $current_dir/
file copy -force $current_dir/tmp/$temp_prj_name/$temp_prj_name.runs/impl_1/${bd_name}_wrapper.ebc $current_dir/
file copy -force $current_dir/tmp/$temp_prj_name/$temp_prj_name.runs/impl_1/${bd_name}_wrapper.ebd $current_dir/
