module Audio_Config (	//	Host Side
						CLOCK_50,
						reset,
						//	I2C Side
						I2C_SCLK,
						I2C_SDAT	);
//	Host Side
input		CLOCK_50;
input		reset;
//	I2C Side
output		I2C_SCLK;
inout		I2C_SDAT;
//	Internal Registers/Wires
reg	[15:0]	mI2C_CLK_DIV;
reg	[23:0]	mI2C_DATA;
reg			mI2C_CTRL_CLK;
reg			mI2C_GO;
wire		mI2C_END;
wire		mI2C_ACK;
wire		iRST_N = !reset;
reg	[15:0]	LUT_DATA;
reg	[5:0]	LUT_INDEX;
reg	[3:0]	mSetup_ST;

parameter USE_MIC_INPUT		= 1'b0;

parameter AUD_LINE_IN_LC	= 9'd24;
parameter AUD_LINE_IN_RC	= 9'd24;
parameter AUD_LINE_OUT_LC	= 9'd119;
parameter AUD_LINE_OUT_RC	= 9'd119;
parameter AUD_ADC_PATH		= 9'd17;
parameter AUD_DAC_PATH		= 9'd6;
parameter AUD_POWER			= 9'h000;
parameter AUD_DATA_FORMAT	= 9'd77;
parameter AUD_SAMPLE_CTRL	= 9'd0;
parameter AUD_SET_ACTIVE	= 9'h001;

//	Clock Setting
parameter	CLK_Freq	=	50000000;	//	50	MHz
parameter	I2C_Freq	=	20000;		//	20	KHz
//	LUT Data Number
parameter	LUT_SIZE	=	50;
//	Audio Data Index
parameter	SET_LIN_L	=	0;
parameter	SET_LIN_R	=	1;
parameter	SET_HEAD_L	=	2;
parameter	SET_HEAD_R	=	3;
parameter	A_PATH_CTRL	=	4;
parameter	D_PATH_CTRL	=	5;
parameter	POWER_ON	=	6;
parameter	SET_FORMAT	=	7;
parameter	SAMPLE_CTRL	=	8;
parameter	SET_ACTIVE	=	9;

/////////////////////	I2C Control Clock	////////////////////////
always@(posedge CLOCK_50 or negedge iRST_N)
begin
	if(!iRST_N)
	begin
		mI2C_CTRL_CLK	<=	0;
		mI2C_CLK_DIV	<=	0;
	end
	else
	begin
		if( mI2C_CLK_DIV	< (CLK_Freq/I2C_Freq) )
		mI2C_CLK_DIV	<=	mI2C_CLK_DIV+1;
		else
		begin
			mI2C_CLK_DIV	<=	0;
			mI2C_CTRL_CLK	<=	~mI2C_CTRL_CLK;
		end
	end
end
////////////////////////////////////////////////////////////////////
I2C_Controller 	u0	(	.CLOCK(mI2C_CTRL_CLK),		//	Controller Work Clock
						.I2C_SCLK(I2C_SCLK),		//	I2C CLOCK
 	 	 	 	 	 	.I2C_SDAT(I2C_SDAT),		//	I2C DATA
						.I2C_DATA(mI2C_DATA),		//	DATA:[SLAVE_ADDR,SUB_ADDR,DATA]
						.GO(mI2C_GO),      			//	GO transfor
						.END(mI2C_END),				//	END transfor 
						.ACK(mI2C_ACK),				//	ACK
						.RESET(iRST_N)	);
////////////////////////////////////////////////////////////////////
//////////////////////	Config Control	////////////////////////////
always@(posedge mI2C_CTRL_CLK or negedge iRST_N)
begin
	if(!iRST_N)
	begin
		LUT_INDEX	<=	0;
		mSetup_ST	<=	0;
		mI2C_GO		<=	0;
	end
	else
	begin
		if(LUT_INDEX<LUT_SIZE)
		begin
			case(mSetup_ST)
			0:	begin
					if(LUT_INDEX<10)
					mI2C_DATA	<=	{8'h34,LUT_DATA};
					else
					mI2C_DATA	<=	{8'h40,LUT_DATA};
					mI2C_GO		<=	1;
					mSetup_ST	<=	1;
				end
			1:	begin
					if(mI2C_END)
					begin
						if(!mI2C_ACK)
						mSetup_ST	<=	2;
						else
						mSetup_ST	<=	0;							
						mI2C_GO		<=	0;
					end
				end
			2:	begin
					LUT_INDEX	<=	LUT_INDEX+1;
					mSetup_ST	<=	0;
				end
			endcase
		end
	end
end
////////////////////////////////////////////////////////////////////
/////////////////////	Config Data LUT	  //////////////////////////	
always
begin
	case(LUT_INDEX)
	//	Audio Config Data
	SET_LIN_L	:	LUT_DATA	<=	{7'h0, AUD_LINE_IN_LC};
	SET_LIN_R	:	LUT_DATA	<=	{7'h1, AUD_LINE_IN_RC};
	SET_HEAD_L	:	LUT_DATA	<=	{7'h2, AUD_LINE_OUT_LC};
	SET_HEAD_R	:	LUT_DATA	<=	{7'h3, AUD_LINE_OUT_RC};
	A_PATH_CTRL	:	LUT_DATA	<=	{7'h4, AUD_ADC_PATH} + (16'h0004 * USE_MIC_INPUT);
	D_PATH_CTRL	:	LUT_DATA	<=	{7'h5, AUD_DAC_PATH};
	POWER_ON	:	LUT_DATA	<=	{7'h6, AUD_POWER};
	SET_FORMAT	:	LUT_DATA	<=	{7'h7, AUD_DATA_FORMAT};
	SAMPLE_CTRL	:	LUT_DATA	<=	{7'h8, AUD_SAMPLE_CTRL};
	SET_ACTIVE	:	LUT_DATA	<=	{7'h9, AUD_SET_ACTIVE};
	endcase
end
////////////////////////////////////////////////////////////////////
endmodule