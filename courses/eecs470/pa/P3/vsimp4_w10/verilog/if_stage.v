/////////////////////////////////////////////////////////////////////////
//                                                                     //
//   Modulename :  if_stage.v                                          //
//                                                                     //
//  Description :  instruction fetch (IF) stage of the pipeline;       // 
//                 fetch instruction, compute next PC location, and    //
//                 send them down the pipeline.                        //
//                                                                     //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

`timescale 1ns/100ps

module if_stage(// Inputs
                clock,
                reset,
                mem_wb_valid_inst,
                ex_mem_take_branch,
                ex_mem_target_pc,
                Imem2proc_data,

								id_raw_stall,
								ex_mem_rd_mem,
								ex_mem_wr_mem,
								ex_mem_valid_inst,
								
                    
                // Outputs
                if_NPC_out,        // PC+4 of fetched instruction
                if_IR_out,         // fetched instruction out
                proc2Imem_addr,
                if_valid_inst_out  // when low, instruction is garbage
               );

  input         clock;              // system clock
  input         reset;              // system reset
  input         mem_wb_valid_inst;  // only go to next instruction when true
                                    // makes pipeline behave as single-cycle
  input         ex_mem_take_branch; // taken-branch signal
  input  [63:0] ex_mem_target_pc;   // target pc: use if take_branch is TRUE
  input  [63:0] Imem2proc_data;     // Data coming back from instruction-memory

  input			id_raw_stall;	// Whether stall increasing PC due to raw detected by id
	input			ex_mem_rd_mem;
	input			ex_mem_wr_mem;
	input			ex_mem_valid_inst;

  output [63:0] proc2Imem_addr;     // Address sent to Instruction memory
  output [63:0] if_NPC_out;         // PC of instruction after fetched (PC+4).
  output [31:0] if_IR_out;          // fetched instruction
  output        if_valid_inst_out;

  reg    [63:0] PC_reg;               // PC we are currently fetching
  //reg           if_valid_inst_out;
	//reg		 [31:0]	if_IR_out;

  wire   [63:0] PC_plus_4;
  wire   [63:0] next_PC;
  wire          PC_enable;
	wire					mem_struct_hazard;
   
  assign proc2Imem_addr = {PC_reg[63:3], 3'b0};

    // this mux is because the Imem gives us 64 bits not 32 bits
	/*always @(posedge clock)
	begin
			if (reset)
					if_IR_out <= `SD `NOOP_INST;
			else
 				if_IR_out <= `SD PC_reg[2] ? Imem2proc_data[63:32] : Imem2proc_data[31:0];
	end*/
	assign if_IR_out = PC_reg[2] ? Imem2proc_data[63:32] : Imem2proc_data[31:0];

    // default next PC value
  assign PC_plus_4 = PC_reg + 4;

	assign mem_struct_hazard = (ex_mem_rd_mem || ex_mem_wr_mem);

    // next PC is target_pc if there is a taken branch or
    // the next sequential PC (PC+4) if no branch
    // (halting is handled with the enable PC_enable;
  //assign next_PC = ex_mem_take_branch ? ex_mem_target_pc : 
  	//							 id_raw_stall ? PC_reg : PC_plus_5;
	assign next_PC = ex_mem_take_branch ? ex_mem_target_pc : PC_plus_4;

    // The take-branch signal must override stalling (otherwise it may be lost)
  assign PC_enable= (if_valid_inst_out & ~id_raw_stall) 
										| ex_mem_take_branch;
	//assign if_valid_inst_out = (proc2Dmem_command == `BUS_NONE) ? 1 : 0;

    // Pass PC+4 down pipeline w/instruction
  assign if_NPC_out = (if_valid_inst_out)? PC_plus_4 : 0;

  // This register holds the PC value
  // synopsys sync_set_reset "reset"
  always @(posedge clock)
  begin
    if(reset)
      PC_reg <= `SD 0;       // initial PC value is 0
    else if(PC_enable)
      PC_reg <= `SD next_PC; // transition to next PC
  end  // always

    // This FF controls the stall signal that artificially forces
    // fetch to stall until the previous instruction has completed
    // This must be removed for Project 3
  // synopsys sync_set_reset "reset"
	assign if_valid_inst_out = (~mem_struct_hazard || ~ex_mem_valid_inst) ? 1 : 0;
	
 /* always @(posedge clock)
  begin
   if (reset)
     if_valid_inst_out <= `SD 1;  // must start with something
	else
		if_valid_inst_out <= `SD (~mem_struct_hazard || ~ex_mem_valid_inst) ? 1 : 0;

 //   else
   //   if_valid_inst_out <= `SD mem_wb_valid_inst;
  end*/
  
endmodule  // module if_stage
