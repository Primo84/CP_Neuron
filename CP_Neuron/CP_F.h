
#include "Resource.h"
#include "fstream"
#include "stdio.h"
#include "stddef.h"
#include"iostream"
#include "String"
#include "windowsx.h"
#include "math.h"

using namespace std;

//const float n=0.5;    // parametr ni algorytmu uczacego siec CP 

//Struktura zawierajace informacje na temat  parametrow, oraz przebiegu uczenia sieci CP

/*
struct CPU_R
{
	int xc;
	float X[MAX_W];
	int id_n;			//indeks zwycieskiego neuronu pierwszej warstwy, na ktorym wykonano proces uczenia
};
*/
//Struktura naglowka pliku parametrow

struct CP_PARAM_HEADER
{
	int Cpin_count,Cpout_count;
};

//Struktura opisu parametrow wejsciowych

struct CP_IN
{
	wchar_t opis[150];
	char ef[2];
};

//Struktura opisu parametrow wyjsciowych

struct CP_OUT
{

	wchar_t opis[150];
	char ef[2];
};


//Struktura ciagu uczacego

struct CP_TEACHER
{
	float WX[MAX_W];
	int count_x;
	int count_y;
	int WY[MAX_Y];
	char ef[2];
};

// Struktura naglowka pliku neuronow

struct HEADER_N
{
	int neuron_count,TEACHE_C,yc;
	float n,n2,n3;
	int k;
	int kn;
	char ef[2];
};

//Strukturaneuronu

struct CP_NEURON
{
	float WX [MAX_W];
	int count_w;
	float y;
	bool wy;
	char ef[2];
};



		/*
			
				Funkcja tworzaca wagi neuronow i zapisujaca je do pliku

				Parametry:
						
							[in] n_c (int) 				liczba neuronow
							[in] n (float)				wspolczynnik n ciagu uczacego
							[in] w-c (int)				licznba wag neuronow
							[in,out] CPN (CP_NEURON)	wskaznik do tablicy neuronow

							Return (int) :
														0 - CPN = NULL
													   -1 - Nie mozma zaladowac pliku zawierajacego siec neronow 
					
		*/


int Init_Cp(int n_c,float n,int w_c,int yc,CP_NEURON *CPN, CP_NEURON *CPN1)
{
	int i,j,ls,ls1;
	fstream pl;
	CP_NEURON *CP1,*CP2;
	HEADER_N HDN={n_c,0,yc,n,(float)N_2,(float)N_3,0,0,{13,10}};
	SYSTEMTIME LPS;

	if(CPN==NULL||CPN1==NULL) return 0;
	CP1=CPN;
	CP2=CPN1;
	pl.open(CP_FILE,ios_base::in|ios_base::out|ios_base::trunc|ios_base::binary);
	if(!pl) return -1;

	float f=(float)1/w_c;
	f=sqrt(f);
	for(j=0;j<n_c;j++)
	{
			CP1[j].y=0;
			CP1[j].wy=false;
			CP1[j].ef[0]=13;
			CP1[j].ef[1]=10;
			CP1[j].count_w=w_c;


		for(i=0;i<w_c;i++)
		{
			CP1[j].WX[i]=f;
		}
	}

	GetSystemTime(&LPS);
	ls=(int)LPS.wMilliseconds;
	ls=ls<<16;
	ls=ls+LPS.wSecond;
	for(j=0;j<yc;j++)
	{
			CP2[j].count_w=n_c;
			CP2[j].y=0;
			CP2[j].wy=false;
			CP2[j].ef[0]=13;
			CP2[j].ef[1]=10;
		
		for(i=0;i<n_c;i++)	
		{
			srand(ls);
			ls=rand();
			ls1=(ls*10)/RAND_MAX;
			f=(float)ls1;
			f=f/10;
			srand(ls);
			ls=rand();
			ls1=RAND_MAX/2;
			if(ls<ls1) f=f-(f*2);
			CP2[j].WX[i]=f;
		}
		
	}

		
	pl.write((char*)&HDN,sizeof(HEADER_N));

	for(i=0;i<n_c;i++)
	{
		pl.write((char*)CP1,sizeof(CP_NEURON));
		CP1++;
	}
	for(i=0;i<yc;i++)
	{
		pl.write((char*)CP2,sizeof(CP_NEURON));
		CP2++;
	}
	
	pl.close();

	return 1;
}




		/*
			
				Funkcja zapisujaca do pliku wagi neuronow

				Parametry:
						
							[in] HDN (HEADER_N)			wskaznik do struktury naglowka 
							[in] CPN,CPN1 (CP_NEURON)	wskazniki do tablic neuronow

							Return (int) :
														0 - CPN||CPN1 = NULL
													   -1 - Nie mozma zaladowac pliku zawierajacego siec neronow 
													    1 - Powodzenie
					
		*/



int Save_Cp(CP_NEURON *CPN, CP_NEURON *CPN1,HEADER_N HDN)
{
	int i;
	fstream pl;
	CP_NEURON *CP1,*CP2;
	

	if(CPN==NULL||CPN1==NULL) return 0;
	CP1=CPN;
	CP2=CPN1;
	pl.open(CP_FILE,ios_base::in|ios_base::out|ios_base::trunc|ios_base::binary);
	if(!pl) return -1;

	HDN.TEACHE_C=0;
	pl.write((char*)&HDN,sizeof(HEADER_N));

	for(i=0;i<HDN.neuron_count;i++)
	{
		pl.write((char*)CP1,sizeof(CP_NEURON));
		CP1++;
	}
	for(i=0;i<HDN.yc;i++)
	{
		pl.write((char*)CP2,sizeof(CP_NEURON));
		CP2++;
	}
	pl.close();
	return 1;
}



			/*
				
				Funkcja ladujaca wagi neuronow

				Return :
						(CP_NEURON*) zwraca wskaznik do tablicy neuronow
				
			*/


int Load_Cp(CP_NEURON * CPN, CP_NEURON *CPN1)
{
	fstream pl;
	HEADER_N HDN;
	int i;
	CP_NEURON *CP, *CP1;

	pl.open(CP_FILE,ios_base::in|ios_base::binary);
	if(pl.fail()) return 0;
	pl.read((char*)&HDN,sizeof(HEADER_N));
	CP=CPN;
	CP1=CPN1;
	for(i=0;i<HDN.neuron_count;i++)
	{
		pl.read((char*)CP,sizeof(CP_NEURON));
		CP++;
	}
	for(i=0;i<HDN.yc;i++)
	{
		pl.read((char*)CP1,sizeof(CP_NEURON));
		CP1++;
	}

	pl.close();
	return 1;
}


		/*
			
				Funkcja zczytujaca naglowek

				Parametry:
							[in.,out] HEADER_N*	wsakznik do struktury naglowka

				Return:
							(in)

								0-Nie mozna zaladowac pliku bazy
								1- -Powodzenie
				
		*/



int Load_H(HEADER_N * HDN)
{
	fstream pl;


	pl.open(CP_FILE,ios_base::in|ios_base::binary);
	if(!pl) return 0;
	pl.read((char*)HDN,sizeof(HEADER_N));
	pl.close();
	return 1;
}

	/*
		Funkcja zapisuje do pliku ciag uczacy
		
		Parametry:
					CPT (CP_TEACHER) - wskaznik do tablicy ciagu
					count (int) - ilosc tablic do zapisania
		Return :
					(int) :
							0 - nie mozna otworzyc pliku bazy
							1 - zapisano pomyslnie


	*/

int Create_T(CP_TEACHER *CPT,int count)
{
	fstream pl;
	HEADER_N HDN;
	CP_TEACHER *CT;
	int i;

	pl.open(CP_FILE,ios_base::in|ios_base::out|ios_base::binary);
	if(!pl) return 0;
	pl.read((char*)&HDN,sizeof(HEADER_N));
	HDN.TEACHE_C=count;
	pl.seekg(0,ios_base::beg);
	pl.write((char*)&HDN,sizeof(HEADER_N));
	pl.seekg((HDN.neuron_count+HDN.yc) * sizeof(CP_NEURON),ios_base::cur);
	CT=CPT;
	for(i=0;i<count;i++)
	{
		pl.write((char*)CT,sizeof(CP_TEACHER));
		CT++;
	}
	pl.close();

return 1;
}


	/*
		
		Funkcja ladujaca ciag uczacy

			Parametry:
						[in,out] CP_TEACHER*	wskaznik do struktury CP_TEACHER

			Return:
						(int)

							0	-	Nie mozna zaladowac bazy
						   -1	-	Ciag uczacy nie zostal utworzonu
							1	-	Zaladowano pomyslnie

	*/


int Load_T(CP_TEACHER* CPT)
{
	CP_TEACHER *CT;
	HEADER_N HDN;
	fstream pl;
	int i;

	pl.open(CP_FILE,ios_base::in|ios_base::binary);
	if(!pl) return  0;
	pl.read((char*)&HDN,sizeof(HEADER_N));
	if(HDN.TEACHE_C==0)
	{
		return -1;
		pl.close();
	}

	pl.seekg((HDN.neuron_count+HDN.yc) * sizeof(CP_NEURON),ios_base::cur);
	CT=CPT;
	for(i=0;i<HDN.TEACHE_C;i++)
	{
		pl.read((char*)CT,sizeof(CP_TEACHER));
		CT++;

	}
	pl.close();
	return 1;
	
	
}

//Funkcja tworzy Menu podanego okienka



bool CreateWindowMenu(HINSTANCE hinst,HWND hwd)

{
	HBITMAP bmp;
	wchar_t itemstr[]=L"Koniec";

	if(hwd==NULL)
	{
		return false;
	}
	
	HMENU hm = GetMenu(hwd);



	bmp=(HBITMAP)LoadBitmap(hinst,MAKEINTRESOURCE(IDB_1));
	MENUITEMINFO item={sizeof(MENUITEMINFO),MIIM_CHECKMARKS|MIIM_ID|MIIM_TYPE|MIIM_STATE,MFT_STRING,MFS_ENABLED,IDM_KONIEC,NULL,bmp,bmp,NULL,itemstr,_countof(itemstr)};
	InsertMenuItem(GetSubMenu(GetMenu(hwd),0),1,true,&item);
	bmp=(HBITMAP)LoadBitmap(hinst,MAKEINTRESOURCE(IDB_3));
	MENUITEMINFO item1={sizeof(MENUITEMINFO),MIIM_CHECKMARKS|MIIM_ID|MIIM_TYPE|MIIM_STATE,MFT_STRING,MFS_ENABLED,IDM_U_CP,NULL,bmp,bmp,NULL,L"&Stwurz nowa siec",_countof(itemstr)};
	InsertMenuItem(GetSubMenu(GetMenu(hwd),1),1,true,&item1);
	bmp=(HBITMAP)LoadBitmap(hinst,MAKEINTRESOURCE(IDB_2));
	MENUITEMINFO item2={sizeof(MENUITEMINFO),MIIM_CHECKMARKS|MIIM_ID|MIIM_TYPE|MIIM_STATE,MFT_STRING,MFS_DISABLED,IDM_U_CU,NULL,bmp,bmp,NULL,L"&Konfiguruj ciag uczacy",_countof(itemstr)};
	InsertMenuItem(GetSubMenu(GetMenu(hwd),1),2,true,&item2);
	bmp=(HBITMAP)LoadBitmap(hinst,MAKEINTRESOURCE(IDB_4));
	MENUITEMINFO item3={sizeof(MENUITEMINFO),MIIM_CHECKMARKS|MIIM_ID|MIIM_TYPE|MIIM_STATE,MFT_STRING,MFS_DISABLED,IDM_KA,NULL,bmp,bmp,NULL,L"&Opisz atrybuty",_countof(itemstr)};
	InsertMenuItem(GetSubMenu(GetMenu(hwd),1),2,true,&item3);
	bmp=(HBITMAP)LoadBitmap(hinst,MAKEINTRESOURCE(IDB_7));
	MENUITEMINFO item4={sizeof(MENUITEMINFO),MIIM_CHECKMARKS|MIIM_ID|MIIM_TYPE|MIIM_STATE,MFT_STRING,MFS_DISABLED,IDM_AC,NULL,bmp,bmp,NULL,L"&Analiza ciagu uczacego",_countof(itemstr)};
	InsertMenuItem(GetSubMenu(GetMenu(hwd),2),1,true,&item4);



	return true;

}


//Funkcja wyszukuje podany ciag w pliku 

bool find_str(char *fc,LONGLONG*pos,char*ch,int size)
{
	FILE *pf;
	bool bl=false;
	bool bl1=false;
	int i,pos1=0;
	char zn;
	LARGE_INTEGER lg;
	LONGLONG ll,ll1;
	HANDLE fh=0;



	fh=CreateFileA(fc,GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(fh==NULL) 
		return false;
	
	GetFileSizeEx(fh,&lg);
	CloseHandle(fh);
	ll=lg.QuadPart;

	pf=fopen(fc,"r+");

	if(pf==NULL||ch==NULL||size<=0)
		return false;

	ll1=*pos;
	if(ll1>ll||ll1<0)ll1=0;
	fseek(pf,(long)ll1,FILE_BEGIN);
	do
	{

		//fseek(pf,ll1,FILE_BEGIN);
		zn=fgetc(pf);
		
		ll1++;

		
		if((zn==-1)&&(ll!=ll1))
		{
	
			fseek(pf,(long)ll1,FILE_BEGIN);
		}
		
		if(ll==ll1) bl=true;
		if(zn==ch[0])
		{
			i=1;
			do
			{
				zn=(char)fgetc(pf);
				ll1++;
				if(i>2)
				{
					i=i;
					}
				i++;
				if(ll==ll1) bl=true;
			}while((zn==ch[i-1])&&(i<size+1));
			
			if(i==size+1)
			{
				bl=bl1=true;
				*pos=ll1-1-size;
			}
			else 
			{
				ll1--;
				fseek(pf,-1,FILE_CURRENT);
			}
		}

	}while(!bl);
	fclose(pf);
	return bl1;
}


		/*funkcja sprawdza czy program jest uruchamiany pierwszy raz 
		Wczytuje parametry z biblioteki CP_Dll.h

		Zwraca parametry:
		0-  Program byl juz uruchomiony oraz zostaly zainicjowane poczatkowe wagi nauronow
		1-  Program uruchamiany pierwszy, nalezy ustawic poczatkowe wartosci neuronow
		-1- Blad podczas odczytu danych z biblioteki


*/




int Init_Dll()


{
	FILE *f;
	char ch[20]="Start_Dll";
	
	
	LONGLONG pos=0;		
	
	bool bl=true;
	HMODULE hmd;
	char st[10];
	char st2[10]="Start";


	hmd =LoadLibrary(L"Cp_Dll.dll");
	
	if(hmd==NULL) return -1;
	void (*pf1)(char* ch)=(void(*)(char*))GetProcAddress(hmd,"get_str");
	(*pf1)(st);

	FreeLibrary(hmd);

	int i=strcmp(st,st2);		//zwraca 0 jesli ciagi sa rowne
	if(i==0)
	{

		return 0;
	}

	bl=find_str("Cp_Dll.dll",&pos,ch,9);

	
	f= fopen("Cp_Dll.dll","r+");
	
	if(f==NULL)return -1;

	
	fseek(f,(long)pos+10,FILE_BEGIN);

	for(int i=0;i<5;i++)
	
	{
		fputc(st2[i],f);
	}
	
	

	fclose(f);

	return 1;

}



	/*funkcja zapisujaca do pliku opisy parametrow
		
		Parametry:
				CPPH	(CP_PARAM_HEADER)	-	struktura naglowka
				CPI		(CP_IN)				-	wskaznik do struktury wejsc
				CPO		(CP_OUT)			-	wskaznik do struktury wyjsc
				L1		(HWND)				-	uchwyt listy okna 1 zawierajacej opisy wejsc
				L2		(HWND)				-	uchwyt listy okna 2 zawiwerajacej opisy wyjsc
		Return:
				 0	-	gdy parametr wejsciowy jest pusty badz nie prawidlowy
				 1	-	powodzenie
				-1	-	nie mozna utworzyc pliku
		
	*/

int Save_CPIO(CP_PARAM_HEADER CPPH,CP_IN *CPI,CP_OUT *CPO,HWND L1,HWND L2)
{
	int i,i1;
	wchar_t str[150];
	fstream pl;
	CP_IN *CI;
	CP_OUT *CO;
	
	if(CPI==NULL||CPO==NULL||L1==NULL||L2==NULL)
		return 0;

	pl.open("CP_In_Out.cpn",ios_base::out|ios_base::in|ios_base::trunc|ios_base::binary);
	if(!pl)return -1;
	CI=CPI;
	CO=CPO;
	pl.write((char*)&CPPH,sizeof(CP_PARAM_HEADER));
	for(i=0;i<CPPH.Cpin_count;i++)
	{
		SendMessage(L1,LB_GETTEXT,(WPARAM)i,(LPARAM)str);
		for(i1=0;str[i1]!='-';i1++)i1=i1;
		wcscpy(CPI[i].opis,str+i1+3);
		CPI[i].ef[0]=13;
		CPI[i].ef[1]=10;
		pl.write((char*)CI,sizeof(CP_IN));
		CI++;
	}
	for(i=0;i<CPPH.Cpout_count;i++)
	{
		SendMessage(L2,LB_GETTEXT,(WPARAM)i,(LPARAM)str);
		for(i1=0;str[i1]!='-';i1++)i1=i1;
		wcscpy(CPO[i].opis,str+i1+1);
		CPO[i].ef[0]=13;
		CPO[i].ef[1]=10;
		pl.write((char*)CO,sizeof(CP_OUT));
		CO++;
	}
	
	pl.close();
	return 1;
}

	/*
		Funkcja laduje naglowek pliku zawierajacego parametry

			Parametry :	
						CPPH	(CP_PARAM_HEADER)	-	wskaznik do struktury naglowka
			Return:
					-1	- CPPH = NULL
					 0	- nie mozna otworzyc pliku
					 1	- powodzenie 
	*/

int Load_CIO_Header(CP_PARAM_HEADER *CPPH)
{
	fstream pl;

	if(CPPH==NULL)
	return -1;
	pl.open("CP_In_Out.cpn",ios_base::in|ios_base::binary);
	if(!pl)return 0;
	pl.read((char*)CPPH,sizeof(CP_PARAM_HEADER));
	pl.close();
	return 1;

}



/*
		Funkcja laduje opis parametrow sieci z pliku

			Parametry :	
						CPI		(CP_IN)				-	wskaznik do struktury wejsc
						CPO		(CP_OUT)			-	wskaznik do struktury wyjsc
			Return:
					-1	- CPPH = NULL
					 0	- nie mozna otworzyc pliku
					 1	- powodzenie 
	*/



int Load_CIO(CP_IN *CPI,CP_OUT *CPO)
{
	fstream pl;
	CP_PARAM_HEADER CPPH;
	CP_IN *CI;
	CP_OUT *CO;
	int i;

	if(CPI==NULL||CPO==NULL) 
	return -1;
	pl.open("CP_In_Out.cpn",ios_base::in|ios_base::binary);
	if(!pl)return 0;
	pl.read((char*)&CPPH,sizeof(CP_PARAM_HEADER));
	CI=CPI;
	CO=CPO;
	for(i=0;i<CPPH.Cpin_count;i++)
	{
		pl.read((char*)CI,sizeof(CP_IN));
		CI++;
	}
	for(i=0;i<CPPH.Cpout_count;i++)
	{
		pl.read((char*)CO,sizeof(CP_OUT));
		CO++;
	}
	pl.close();
	return 1;

}



			/*Funkcja normalizacji sygnalow wejsciowych

				Parsmetry:
							CPT (CP_TEACHER) - wskaznik do tablicy ciagu uczacego
							HDN (HEADER_N)   - wskaznik do struktury naglowka
						
						(int):
								0 - wskaznik do tablicy jest pusty
								1 - powodzenie
			*/


int Norm_X(CP_TEACHER *CPT,HEADER_N HDN)
{
	float suma,fl;
	int i,j;

	if(CPT==NULL) return 0;
	for(j=0;j<HDN.TEACHE_C;j++)
	{
		suma=0;
		for(i=0;i<CPT[j].count_x;i++)
			suma=suma+CPT[j].WX[i];
		if(suma==0) suma=1;
		for(i=0;i<CPT[j].count_x;i++)
		CPT[j].WX[i]=CPT[j].WX[i]/suma;
	}

	return 1;
}



			/*
				Funkcja przetwarzajaca sygnaly wejsciowe ciagu uczacego

					Parametry:
								CPT (CP_TEACHER*)	- wskaznik do tablicy ciagu uczacego
								HDN (HEADER_N)		- wskaznik do struktury naglowka
					Return:
							(int):
									-1	- wskaznik do tablicy jest pusty
									 0	- Nie mozna zaladowac danych z pliku zawierajacego informacje na temat parametrow uczenia
									 1	- powodzenie
			*/

int Norm_CPT(CP_TEACHER *CPT,HEADER_N HDN)
{
	int i,j;
	int k=HDN.k;
	int cnt1;
	int cnt=HDN.TEACHE_C;
	float x,nk,n2,kn;

	if(CPT==NULL) return -1;
	n2=HDN.n2;
	for(j=0;j<cnt;j++)
	{
		k++;
		cnt1=CPT[j].count_x;
		for(i=0;i<cnt1;i++)
		{
			kn=n2*k;
			if(kn>1) kn=1;
			x=kn*CPT[j].WX[i];
			nk=(float)1/cnt1;
			nk=sqrt(nk);
			nk=nk*(1-kn);
			CPT[j].WX[i]=x+nk;
		}
	}

	return 1;
}

					/*
						Funkcja uczaca siec neyronow
							Parameyry:
										CPN (CP_NEURON*)	- wskaznik do tablicy neuronow pierwszej warstwy
										CPN1(CP_NEURON*)	- wskaznik do tablicy neuronow drugiej warstwy
										CPT (CP_TEACHER*)	- wskaznik do tablicy ciagu uczacego
										HDN (HEADER_N)		- struktura naglowka pliku neuronow
							Return:
									0 - jesli ktorys ze wskaznikow jest pusty
								   -1 - jesli nie mozna zapisac do pliku
									1 - powodzenie
					*/

int Teach_CP(CP_NEURON *CPN, CP_NEURON *CPN1,CP_TEACHER *CPT, HEADER_N HDN)
{
	int i,j,k,id;
	float y,wy,max;

	if(CPN==NULL||CPN1==NULL||CPT==NULL) return 0;
	//Norm_X(CPT,HDN);
	Norm_CPT(CPT,HDN);
	y=0;
	for(k=0;k<HDN.TEACHE_C;k++)
	{
		max=0;
		for(j=0;j<HDN.neuron_count;j++)
		{
			wy=0;
			for(i=0;i<CPN[j].count_w;i++)
			{
				wy=wy+(CPN[j].WX[i]*CPT[k].WX[i]);
			}
			if(j==0)
			{
				max=wy;
				id=j;
			}else
			if(wy>max)
			{
				max=wy;
				id=j;
			}
		}

		if(HDN.kn+1<20) HDN.kn++; 
		else
		{
			HDN.kn=0;
			y=0.1;
			if((HDN.n<0.2)&&(HDN.n>0.1))y=0.01;
			while(y>=HDN.n)
			{
				y=y/10;
			}
			HDN.n=HDN.n-y;
			y=0.1;
			if((HDN.n3<0.2)&&(HDN.n3>0.1))y=0.01;
			while(y>=HDN.n3)
			{
				y=y/10;
			}
			HDN.n3=HDN.n3-y;
		}

		for(i=0;i<CPN[id].count_w;i++)
		{
			y=CPN[id].WX[i]+(HDN.n*(CPT[k].WX[i]-CPN[id].WX[i]));
			CPN[id].WX[i]=y;
		}
		
		for(j=0;j<HDN.yc;j++)
		{
			wy=CPN1[j].WX[id];
			//if(wy<0) wy=0; else wy=1;
			if(CPT[k].WY[j]==0) CPT[k].WY[j]=-1;
			y=HDN.n3*(CPT[k].WY[j]-wy);
			CPN1[j].WX[id]=CPN1[j].WX[id]+y;
		}
	}
	HDN.k=HDN.k+k;
	i=Save_Cp(CPN,CPN1,HDN);
	


	return i;
}


		/*
			Funkcja generuje odpowiedz na podstawie podanych parametrow
				Parametry:
							CPK (CP_NEURON*)	wskaznik do tablicy neuronow
							CPG(CP_NEURON*)		wskaznik do tablicy neuronow
							HDN(HEADER_N)		struktura naglowka
							inp(float*)			tablica parametrow wejsciowych[in]
							out(float*)			tablica parametrow wyjsciowych[out]
							cnt_i,cnt_o(int)	ilosc elementow tablic parametrow
				Return:
						(int):
								-1	- wksaniki do tablic sa puste
								 1	- powodzenie

		*/


int Generate(CP_NEURON *CPK,CP_NEURON *CPG,HEADER_N HDN,float *inp)
{
	int i,j,id,id1;
	float suma,fl,max,wy;

	if(CPK==NULL||CPG==NULL||inp==NULL) return -1;
	suma=0;
	for(i=0;i<CPK[i].count_w;i++)
	{
		suma=suma+inp[i];
	}
	if(suma==0) suma=1;
	//suma=sqrt(suma);
	for(i=0;i<CPK[i].count_w;i++)

	{
		inp[i]=inp[i]/suma;
	}
	max=0;
	for(j=0;j<HDN.neuron_count;j++)
	{
		wy=0;
		for(i=0;i<CPK[j].count_w;i++)
		{
			wy=wy+(CPK[j].WX[i]*inp[i]);
		}
		if(j==0)
		{
			max=wy;
			id=j;
		}else
		if(wy>max)
		{
			max=wy;
			id=j;
		}
	}
	
	for(j=0;j<HDN.yc;j++)
	{
		wy=CPG[j].WX[id];
		if(j==0)
		{
			id1=j;
			max=wy;
		}else
		{
			if(wy>max)
			{
				max=wy;
				id1=j;
			}
		}
	}
	return id1;
}



