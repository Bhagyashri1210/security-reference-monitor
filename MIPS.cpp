#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536
class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {
		int r1;
		int r2;
		int wreg;
		r1 = (int)(RdReg1.to_ulong()); 
		r2 = (int)(RdReg2.to_ulong());
		wreg = (int)(WrtReg.to_ulong());

		cout<<"r1 "<<r1;
		cout<<"r2 "<<r2;
		cout<<"wr "<<wreg;

		bitset<32> ar1(Registers[r1]);
		ReadData1=ar1;
		cout<<"registers value1"<<ReadData1;

	        bitset<32> br2(Registers[r2]);
		ReadData2=br2;
		cout<<" registers value2"<<ReadData2;
		
                cout<<"write enable "<<WrtEnable;
		if(WrtEnable[0]==1)
		{
                  Registers[wreg]=WrtData;
		  cout<<"writre register contents"<<Registers[wreg];
		  bitset<32> ar1(Registers[r1]);
		 ReadData1=ar1;
		cout<<"registers value1"<<ReadData1;

	        bitset<32> br2(Registers[r2]);
		ReadData2=br2;
		cout<<" registers value2"<<ReadData2;
		 
                }
		
		
		                
         }
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class myclass
{ 
	public:
		bitset<32> adp;
		bitset<32> PcAdd( bitset<32> counter)
			{
			 int count;
	
		         count=(int)(counter.to_ulong());
			 count = count + 4;
			 bitset<32> rp(count);
			 return rp;
			}
		bitset<32> concat(bitset<32> fpc,bitset<32> instruc)
			{
			 string s11 = fpc.to_string(); //pc to string
	   		 string s12 = instruc.to_string(); //inst to string
			 string s13 ="00";
			 string conc = s11.substr(0,4) + s12.substr(6,26)+s13;
			 bitset<32> jp(conc);
			 return jp;
			}
		bitset<32> adder1(bitset<32> dpc,bitset<32> instruc1)
			{
			 string final;
			 string s22=instruc1.to_string();
			 string signex = s22.substr(16,16);
			 bitset<16> a(signex);
			 int sum;
			
			 if(a[16]==1)
			{
				final = "11111111111111" + signex;
			}
			else if(a[16]==0)
			{
				 final ="00000000000000" + signex;
			}
			 
			 string conc1 = final + "00";
			 bitset<32> final1(conc1);
                         int addint;
			 int dpcint;
		         addint=(int)(final1.to_ulong());
			 dpcint=(int)(dpc.to_ulong());
			 sum = dpcint + addint;
			 bitset<32> sum1(sum);
			 return sum1;
			 			
			}

};
class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 
		int op1;
		int op2;
		int res;
                string Aluops;
		Aluops = ALUOP.to_string();	
		if(!Aluops.compare("001"))
		{	cout<<"sum";
			op1 = (int)(oprand1.to_ulong());
			op2 = (int)(oprand2.to_ulong());
			res=op1+op2;
			cout<<"add result        "<<res;
			bitset<32> res1(res);
			ALUresult = res1;
			}
		if(!Aluops.compare("011"))
		{	cout<<"sub";
			op1 = (int)(oprand1.to_ulong());
			op2 = (int)(oprand2.to_ulong());
			if(op1>op2){
			res=op1-op2;
				}
                        else if(op1<op2){ res=op2-op1;
				}
			bitset<32> res2(res);
			ALUresult = res2;
			}
		if(!Aluops.compare("100"))
		{	cout<<"and";
			op1 = (int)(oprand1.to_ulong());
			op2 = (int)(oprand2.to_ulong());
			res=op1&op2;
			bitset<32> res3(res);
			ALUresult = res3;
			}
		if(!Aluops.compare("111"))
		    {	cout<<"nor";
			op1 = (int)(oprand1.to_ulong());
			op2 = (int)(oprand2.to_ulong());
			res=!(op1|op2);
			bitset<32> res4(res);
			ALUresult = res4;
			}
		if(!Aluops.compare("101"))
		    {	cout<<"or";
			op1 = (int)(oprand1.to_ulong());
			op2 = (int)(oprand2.to_ulong());
			res=op1|op2;
			bitset<32> res5(res);
			ALUresult = res5;
			}
	
		 
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
			i++;
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {
	        cout<<"in read"; 
    		int int_pc;
		int_pc=(int)(ReadAddress.to_ulong());
		bitset<8> pc1; 
		bitset<8> pc2;
		bitset<8> pc3; 
		bitset<8> pc4;
	
		pc1 = IMem[int_pc];
		pc2 = IMem[int_pc + 1];
		pc3 = IMem[int_pc + 2];
		pc4 = IMem[int_pc + 3];
		
                
		cout<<"inst"<<pc1;
		cout<<"inst2"<<pc2;
		cout<<"inst3"<<pc3;
		cout<<"inst4"<<pc4;
		
		
           string s1 = pc1.to_string();
	   string s2 = pc2.to_string();
           string s3 = pc3.to_string();
           string s4 = pc4.to_string();
	
		s1.append(s2);
		s1.append(s3);
		s1.append(s4);

		cout<<"string"<<s1;
		std::bitset<32> Instruction(s1);
		cout<<"instructi"<<Instruction;
                return Instruction;     
              }     
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem()
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
              
		if(readmem[0]==1){
		int int_pc1;
		int_pc1=(int)(Address.to_ulong());
		bitset<8> pc11; 
		bitset<8> pc21;
		bitset<8> pc31; 
		bitset<8> pc41;
	
		pc11 = DMem[int_pc1];
		pc21 = DMem[int_pc1 + 1];
		pc31 = DMem[int_pc1 + 2];
		pc41 = DMem[int_pc1 + 3];
		
                
		cout<<"dinst"<<pc11;
		cout<<"dinst2"<<pc21;
		cout<<"dinst3"<<pc31;
		cout<<"dinst4"<<pc41;
		
		
           string s1 = pc11.to_string();
	   string s2 = pc21.to_string();
           string s3 = pc31.to_string();
           string s4 = pc41.to_string();
	
		s1.append(s2);
		s1.append(s3);
		s1.append(s4);

		cout<<"stringdat"<<s1;
		std::bitset<32> readdata(s1);
		cout<<"instructi Read "<<readdata;
		return readdata;
                }               
		else if(writemem[0]==1)
			{
			  bitset<32> rwritedata;
			  rwritedata= WriteData;
			  int int_pc2;
			  int_pc2=(int)(Address.to_ulong());
			  string w1 = WriteData.to_string();
			  bitset<8> w11(w1.substr(0,8));
			  bitset<8> w21(w1.substr(8,8));
			  bitset<8> w31(w1.substr(16,8)); 
		          bitset<8> w41(w1.substr(24,8));
			
			 DMem[int_pc2]=w11;
			 DMem[int_pc2 + 1]=w21;
			 DMem[int_pc2 + 2]=w31;
			 DMem[int_pc2 + 3]=w41;
			 cout<<"store is over"<<'\n';
			return rwritedata;
			  
			}
              
		     
          }   
                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  


   
int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    myclass me;
RF r1;
 ALU a1;
    int i=0;
    bitset<32> pc;
    bitset<32> Instfinal;


    while (1)
	{
        // Fetch
		 INSMem i1;
                 Instfinal=i1.ReadMemory(pc);
		 cout<<"instfinal"<<Instfinal;
		 bitset<5> Reg1; 
		 bitset<5> Reg2;
		 bitset<5> WReg; 
		 bitset<32> WData;
		 bitset<1> WEnable(0);

	// decode 
		string IFinal = Instfinal.to_string();
		if(!IFinal.compare("11111111111111111111111111111111")){
			break;	}

		string split=IFinal.substr(0,6);
		 cout<<"split"<<split;
			
		 
	//R Type	
		if(!split.compare("000000"))
			{
		 	string Rsplit = IFinal.substr(29,3);
			cout<<"Rsplit"<<Rsplit;
				string rd1s = IFinal.substr(6,5); cout<<"rd1s"<<rd1s;
                                string rd2s = IFinal.substr(11,5);cout<<"rd2s"<<rd2s;
				string wds = IFinal.substr(16,5); cout<<"wds"<<wds;
			
				 bitset<5> RReg1(rd1s); 
				 bitset<5> RReg2(rd2s);
				 bitset<5> WRReg(wds); 
				 bitset<32> WRData(0);
				 bitset<1> WREnable;
				 WREnable = 0;
				 r1.ReadWrite(RReg1,RReg2,WRReg,WRData,WREnable);
				 
				
			if(!Rsplit.compare("001"))
				{
				cout<<"addu"<<'\n';
				bitset<32> oprnd1R(r1.ReadData1);
				bitset<32> oprnd2R(r1.ReadData2);
					
				bitset<32> ALURRES;
				bitset<3> aop(Rsplit);

				ALURRES =a1.ALUOperation(aop,oprnd1R,oprnd2R);
				cout<<"ALURRES "<<ALURRES<<'\n';
				WREnable = 1;
				r1.ReadWrite(RReg1,RReg2,WRReg,ALURRES,WREnable);
				pc=me.PcAdd(pc);
							
				}
			else if(!Rsplit.compare("011"))
				{
				//subu
				cout<<"subu"<<'\n';
				bitset<32> oprd1R(r1.ReadData1);
				bitset<32> oprd2R(r1.ReadData2);
					
				bitset<32> ALURRESS;
				bitset<3> ap(Rsplit);

				ALURRESS =a1.ALUOperation(ap,oprd1R,oprd2R);
				cout<<"ALURRESS "<<ALURRESS<<'\n';
				WREnable = 1;
				r1.ReadWrite(RReg1,RReg2,WRReg,ALURRESS,WREnable);
				pc=me.PcAdd(pc);
				}
			else if(!Rsplit.compare("100"))
				{
				//and
				cout<<"and"<<'\n';
				bitset<32> opd1R(r1.ReadData1);
				bitset<32> opd2R(r1.ReadData2);
					
				bitset<32> ALURRESA;
				bitset<3> apa(Rsplit);

				ALURRESA =a1.ALUOperation(apa,opd1R,opd2R);
				cout<<"ALURRESA "<<ALURRESA<<'\n';
				WREnable = 1;
				r1.ReadWrite(RReg1,RReg2,WRReg,ALURRESA,WREnable);
				pc=me.PcAdd(pc);
				}

			else if(!Rsplit.compare("101"))
				{
				//or
				cout<<"or"<<'\n';
				bitset<32> pd1R(r1.ReadData1);
				bitset<32> pd2R(r1.ReadData2);
					
				bitset<32> ALURRESC;
				bitset<3> apc(Rsplit);

				ALURRESC =a1.ALUOperation(apc,pd1R,pd2R);
				cout<<"ALURRESC "<<ALURRESC<<'\n';
				WREnable = 1;
				r1.ReadWrite(RReg1,RReg2,WRReg,ALURRESC,WREnable);
				pc=me.PcAdd(pc);
				}
			else if(!Rsplit.compare("111"))
				{
					//nor
				cout<<"nor"<<'\n';
				bitset<32> p1R(r1.ReadData1);
				bitset<32> p2R(r1.ReadData2);
					
				bitset<32> ALURRESR;
				bitset<3> apn(Rsplit);

				ALURRESR =a1.ALUOperation(apn,p1R,p2R);
				cout<<"ALURRESR "<<ALURRESR<<'\n';
				WREnable = 1;
				r1.ReadWrite(RReg1,RReg2,WRReg,ALURRESR,WREnable);
				pc=me.PcAdd(pc);
				}
			  else if(!((!Rsplit.compare("001"))&&(!Rsplit.compare("011"))&&(!Rsplit.compare("100"))&&(!Rsplit.compare("101"))&&(!Rsplit.compare("111"))))	
				{
				 cout<<"invalid instruction R";
				 pc=me.PcAdd(pc);
				}
				
			}

		if(split.compare("000000"))
		    {  if(!split.compare("001001")) 
			  {
				//addiu
				cout<<"addiu";
				string Iregs = IFinal.substr(6,5); cout<<"rd1s addiu"<<Iregs ;
                                 string Iregs2 = IFinal.substr(11,5);cout<<"rd2s addiu"<<Iregs2;
				 bitset<5> RegI1(Iregs); 
				 bitset<5> RegI2(Iregs2);
				 bitset<5> WRegI(Iregs2); 
				 bitset<32> WDataI(0);
				 bitset<1> WEnable;
				 WEnable = 0;
				 r1.ReadWrite(RegI1,RegI2,WRegI,WDataI,WEnable);

				 bitset<32> Ioprnd1(r1.ReadData1);
				 
				 string imm ;
				 imm = IFinal.substr(16,16);
				 bitset<32> immb(imm);
				 int It;
				 int immint;
				 int sum;
				 
				 It=(int)(Ioprnd1.to_ulong());
				 immint=(int)(immb.to_ulong());
				 sum = It + immint;
				 bitset<32> sumb(sum);
				 WEnable=1;
				 r1.ReadWrite(RegI1,RegI2,WRegI,sumb,WEnable);
				 pc=me.PcAdd(pc);
		 			
								
			  }

		      else if(!split.compare("000100"))
				{ 
					//beq
				 cout<<"beq";
				
				 string rdregs = IFinal.substr(6,5); cout<<"rd1s beq"<<rdregs;
                                 string rdregs2 = IFinal.substr(11,5);cout<<"rd2s beq"<<rdregs2;
				 bitset<5> Regq1(rdregs); 
				 bitset<5> Regq2(rdregs2);
				 bitset<5> WRegq(rdregs2); 
				 bitset<32> WDataq(0);
				 bitset<1> WEnable;
				 WEnable = 0;
				 r1.ReadWrite(Regq1,Regq2,WRegq,WDataq,WEnable);
				 bitset<32> beqoprnd1(r1.ReadData1);
				 bitset<32> beqoprnd2(r1.ReadData2);

								
			 string beq1=beqoprnd1.to_string() ;
			 string beq2= beqoprnd2.to_string();
			if (!beq1.compare(beq2))
				{ 
					pc=me.PcAdd(pc);//pc=pc+4
					pc=me.adder1(pc,Instfinal);
						
				}
			 else { 
				pc=me.PcAdd(pc);
				}
			
				}
			
			else if(!split.compare("100011"))
			       { 
				cout<<"load word";
				string ld1s = IFinal.substr(6,5); cout<<"ld1s"<<ld1s;
                                string ld2s = IFinal.substr(11,5);cout<<"ld2s"<<ld2s;
				 bitset<5> Reg1(ld1s); 
				 bitset<5> Reg2(ld2s);
				 bitset<5> WReg(ld2s); 
				 bitset<32> WData(0);
				 bitset<1> WEnable;
				 WEnable = 0;
				 r1.ReadWrite(Reg1,Reg2,WReg,WData,WEnable);

				bitset<32> oprnd1(r1.ReadData1);
			 string final;
			 string signex1 = IFinal.substr(16,16);
			 bitset<16> b(signex1);
			 
			 cout<<" the load string     "<<signex1;
			 if(b[16]==1)
			{
				final = "1111111111111111" + signex1;
			}
			else if(b[16]==0)
			{
				 final ="0000000000000000" + signex1;
			}
			 
			bitset<32> oprnd2(final);
			cout<<" the load string final    "<<oprnd2;
			string alu1 ="001";		
			bitset<3> ALOP(alu1);
               //ALU BLOCK
				bitset<32> ALURES;
				ALURES =a1.ALUOperation(ALOP,oprnd1,oprnd2);
				cout<<"ALURES "<<ALURES;
              //data read
				 
				bitset<32> riteData(0);
				bitset<1> readme(1);
				bitset<1> writeme(0);
				bitset<32> MemoryAccess1;
				MemoryAccess1=myDataMem.MemoryAccess(ALURES, riteData,readme, writeme);
				cout<<'\n'<<"Load Memory value"<<MemoryAccess1<<'\n';

            //WRite Back to RF; 
				WEnable=1;
				r1.ReadWrite(Reg1,Reg2,WReg,MemoryAccess1,WEnable);
				cout<<" LOAD READDDDDDDDDDDD"<<r1.ReadData1;
				cout<<" LOAD READDDD 2   "<<r1.ReadData2;
				pc=me.PcAdd(pc);

							
				}



				else if(!split.compare("101011"))
				{
				 //store word
				 cout<<"store word";			
				 string sd1s = IFinal.substr(6,5); cout<<"sd1s"<<sd1s;
                                 string sd2s = IFinal.substr(11,5);cout<<"sd2s"<<sd2s;
				 bitset<5> Rseg1(sd1s); 
				 bitset<5> Rseg2(sd2s);
				 bitset<5> WsReg(sd2s); 
				 bitset<32> WsData(0);
				 bitset<1> WsEnable;
				 WEnable = 0;
				 r1.ReadWrite(Rseg1,Rseg2,WsReg,WsData,WsEnable);
				 bitset<32> oprnsd1(r1.ReadData1);
			 	 string finals;
			 	 string signexs1 = IFinal.substr(16,16);
			 	 bitset<16> c(signexs1);
			 
			 if(c[16]==1)
			{
				finals = "1111111111111111" + signexs1;
			}
			else if(c[16]==0)
			{
				 finals ="0000000000000000" + signexs1;
			}
			 
					bitset<32> oprnsd2(finals);
					string alu2 ="001";
					bitset<3> ALsOP(alu2);
//ALU BLOCK

				bitset<32> ALUsRES;
				ALUsRES =a1.ALUOperation(ALsOP,oprnsd1,oprnsd2);
				cout<<"ALUsRES  "<<ALUsRES;
//data write
				 
bitset<32> sriteData(r1.ReadData2);
bitset<1> readmes(0);
bitset<1> writemes(1);
bitset<32> MemoryAccess2;
MemoryAccess2=myDataMem.MemoryAccess(ALUsRES, sriteData,readmes, writemes);
cout<<"mem 2"<<MemoryAccess2<<'\n';

				 pc=me.PcAdd(pc);

							
				}											

					else if(!split.compare("000010"))
						{ 
							 bitset<32> inpc(me.PcAdd(pc));
							 pc=me.concat(inpc,Instfinal);
							 cout<<"the jump address is  "<<pc;
						}


			
        		   else { 
				cout<<"invalid instruction I";
				 pc=me.PcAdd(pc);
				}
			}
	
		
        myRF.OutputRF(); // dump RF;    
    }
        myDataMem.OutputDataMem(); // dump data mem
      
        return 0;
        
}
