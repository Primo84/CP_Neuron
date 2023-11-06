
#include "CP_F.h"
#include "commctrl.h"
#include "richedit.h"
//#include "string.h"

#define		ID_L1		500
#define		ID_L2		501
#define		ID_ET1		502
#define		ID_Bt_P		503
#define		ID_BtF		504
#define		ID_BtR		505
#define		ID_Bte		506
#define		ID_Bt_clr	507

HWND M_Wnd,Dlg1,Dlg2,Lst1,Lst2,Re_Wnd,St1,ET1,Bt_Przyp,BtF,BtR,BtEx,BtClr;


int wc=-1;
int nc=-1;
int yc=-1;
float ni=-1;
bool Init=true;
bool param =false;
bool zap_cu =true;
float input[MAX_W];
float *output;

int k;

CP_NEURON *CPK,*CPG;
HEADER_N HDN;
CP_TEACHER CPT[MAX_T];
CP_TEACHER *CPT1;
CP_PARAM_HEADER CPPH;
CP_IN *CPI;
CP_OUT *CPO;
//CPU_R *CPUR;

int teach_c;



		// Funkcja inicjuje kontrolki okna glownego aplikacji

bool Init_ctlr(HWND hwd,HINSTANCE hst)
{
	bool bl=true;
	int i=500;
	wchar_t str[150];

	Lst1=CreateWindowEx(WS_EX_DLGMODALFRAME,L"ListBox",L"Parametry wejsciowe",LBS_NOTIFY| WS_BORDER | LBS_DISABLENOSCROLL |WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL,5,20,325,125,hwd,(HMENU)ID_L1,hst,0);
	Lst2=CreateWindowEx(WS_EX_DLGMODALFRAME,L"ListBox",L"Parametry wyjsciowe",LBS_NOTIFY | WS_BORDER | LBS_DISABLENOSCROLL |WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL,465,20,325,125,hwd,(HMENU)ID_L2,hst,0);
	Re_Wnd=CreateWindowEx(WS_EX_DLGMODALFRAME,L"RichEdit20A",L"",ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL,3,275,785,200,hwd,NULL,hst,0);
	St1=CreateWindow(L"Edit",L"Parametr---",WS_CHILD | WS_VISIBLE | ES_READONLY,10,140,80,20,hwd,NULL,hst,0);
	ET1=CreateWindowEx(WS_EX_DLGMODALFRAME,L"Edit",L"",WS_CHILD | WS_VISIBLE,100,140,50,25,hwd,(HMENU)ID_ET1,hst,0);
	Bt_Przyp=CreateWindowEx(WS_EX_DLGMODALFRAME,L"Button",L"Przypisz",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,160,140,80,30,hwd,(HMENU)ID_Bt_P,hst,0);
	BtR=CreateWindowEx(WS_EX_DLGMODALFRAME,L"Button",L"<<",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,250,140,30,30,hwd,(HMENU)ID_BtR,hst,0);
	BtF=CreateWindowEx(WS_EX_DLGMODALFRAME,L"Button",L">>",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,290,140,30,30,hwd,(HMENU)ID_BtF,hst,0);
	BtEx=CreateWindowEx(WS_EX_DLGMODALFRAME,L"Button",L"Wykonaj",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,240,180,80,30,hwd,(HMENU)ID_Bte,hst,0);
	BtClr=CreateWindowEx(WS_EX_DLGMODALFRAME,L"Button",L"Wyczysc",WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,5,235,80,30,hwd,(HMENU)ID_Bt_clr,hst,0);
	
	if(Lst1==NULL||Lst2==NULL) bl=false;
	if(CPI!=NULL)
	{
		for(i=0;i<CPPH.Cpin_count;i++)
		{
			memset(str,0,sizeof(str));
			wcscpy(str,L"X");
			_itow(i+1,str+1,10);
			wcscpy(str+wcslen(str),L"-");
			wcscpy(str+wcslen(str),CPI[i].opis);
			wcscpy(str+wcslen(str),L"=");
			SendMessage(Lst1,LB_ADDSTRING,0,(LPARAM)str);
		}
		SetWindowTextA(St1,"Parametr X1");
		SendMessage(Lst1,LB_SETCURSEL,0,0);
	}

	if(CPO!=NULL)
	{
		for(i=0;i<CPPH.Cpout_count;i++)
		{
			memset(str,0,sizeof(str));
			wcscpy(str,L"Y");
			_itow(i+1,str+1,10);
			wcscpy(str+wcslen(str),L"-");
			wcscpy(str+wcslen(str),CPO[i].opis);
			SendMessage(Lst2,LB_ADDSTRING,0,(LPARAM)str);
		}
	}

	return bl;
}


	//Procedura okna Dialogowe tworzenia sieci neuronow

INT_PTR CALLBACK Dlg_Init(HWND hwd,UINT code, WPARAM wp, LPARAM lp)
{
	char E3_Text[20];
	char E2_Text[20];
	char E1_Text[20];
	char E4_Text[20];
	char info[50]="Maksymalna ilosc wag to :";
	char info1[50]="Maksymalna ilosc mozliwosci wyboru to :";
	char *s;
	int i=0;
	switch (code)
	{
	case WM_INITDIALOG:
		{
			ShowWindow(hwd,SW_NORMAL);
			SetFocus(GetDlgItem(hwd,IDD_2B));
			break;
		}

	case WM_COMMAND:
		{

			if(LOWORD(wp)==IDD_1B)
			{
				memset(E1_Text,0,20);
				memset(E2_Text,0,20);
				memset(E3_Text,0,20);
				memset(E4_Text,0,20);
				GetWindowTextA(GetDlgItem(hwd,IDD_1E),E1_Text,20);
				GetWindowTextA(GetDlgItem(hwd,IDD_2E),E2_Text,20);
				GetWindowTextA(GetDlgItem(hwd,IDD_3E),E3_Text,20);
				GetWindowTextA(GetDlgItem(hwd,IDD_4E),E4_Text,20);

				if((E1_Text[0]==0)||(E2_Text[0]==0)||(E3_Text[0]==0)||(E4_Text[0]==0))
				{
					MessageBox(hwd,L"Pole Nie moze byc puste",L"OSTRZERZENIE",MB_OK);
					break;
				}
			

				wc=atoi(E2_Text);
				nc=atoi(E1_Text);
				yc=atoi(E4_Text);
				ni=(float)atof(E3_Text);

				if(ni<=0||ni>1)
				{
					MessageBox(hwd,L"Nieprawidlowa wrtosc ni\nPodaj wartosc z przedzialu od 0.1 do 1",L"OSTRZERZENIE",MB_OK);
					break;
				}
				if(yc>MAX_Y)
				{
					s=info1;
					s=s+strlen(info1);
					itoa(MAX_Y,s,10);
					MessageBoxA(hwd,info1,"OSTRZERZENIE",MB_OK);
					break;
				}

				if(wc>MAX_W)
				{
					s=info;
					s=s+strlen(info);
					itoa(MAX_W,s,10);
					MessageBoxA(hwd,info,"OSTRZERZENIE",MB_OK);
					break;
				}

				Init=true;
				
				DestroyWindow(hwd);

				
			}
			
			if(LOWORD(wp)==IDD_2B)
			{
				
				DestroyWindow(hwd);
				
			}


			break;
		}

	case WM_CLOSE:
		{
			DestroyWindow(hwd);
			break;
		}
	case WM_DESTROY:
		{
			EnableWindow(M_Wnd,true);
			PostQuitMessage(0);

		}

	default:
		{
			return DefDlgProc(M_Wnd,code,wp,lp);
		}
	}

return 0;
	
}



		//Procedura okna dialogowego opisu wartosci


INT_PTR CALLBACK Dlg1_Init(HWND hwd,UINT code, WPARAM wp, LPARAM lp)
{
	int i=0;
	static int pos;
	char chr[75]="X1";
	char ch[75];
	bool bl=false;
	RECT rec;

	switch (code)
	{
	case WM_INITDIALOG:
		{
			Dlg1=hwd;
			pos=0;
			//memset(ch,0,sizeof(ch));
			SetFocus(GetDlgItem(hwd,IDD_5E));
			for(i=0;i<wc;i++)
			{
				memset(chr+1,0,9);
				itoa(i+1,chr+1,10);
				strcpy(chr+strlen(chr),"---");
				if(bl)
				{
					i=i;
				}
				SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_ADDSTRING,NULL,(LPARAM)chr);	
			}
			SendMessage(GetDlgItem(hwd,IDD_LS1),LB_SETCURSEL,0,0);
			SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_GETTEXT,0,(LPARAM)ch);
			for(i=0;(i<strlen(ch))&&(ch[i]!='-');i++)i=i;
			memset(chr,0,sizeof(chr));
			strcpy(chr,ch);
			memset(ch+i,0,sizeof(ch)-i);
			SetWindowTextA(GetDlgItem(hwd,IDD2_4E),ch);
			if(chr[i+3]!=0)
			{
				memset(ch,0,sizeof(ch));
				strcpy(ch,chr+i+3);
				SetWindowTextA(GetDlgItem(hwd,IDD_5E),ch);
			}
			ShowWindow(hwd,SW_NORMAL);
			break;
		}


	case WM_COMMAND:
		{
			if(LOWORD(wp)==IDD_5E)
			{
				if(HIWORD(wp)==EN_CHANGE)
				{
					memset(ch,0,sizeof(ch));
					strcpy(ch,"X");
					itoa(pos+1,ch+1,10);
					strcpy(ch+strlen(ch),"---");
			 		GetWindowTextA(GetDlgItem(hwd,IDD_5E),chr,sizeof(chr));
					strcpy(ch+strlen(ch),chr);
					SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_DELETESTRING,(WPARAM)pos,0);
					SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_INSERTSTRING,(WPARAM)pos,(LPARAM)ch);
				}
				//break;
			}
			if(wp==IDD_3B)
			{
				
				if(pos<SendMessage(GetDlgItem(hwd,IDD_LS1),LB_GETCOUNT,0,0)-1)
				{
					SendMessage(GetDlgItem(hwd,IDD_LS1),LB_SETCURSEL,(WPARAM)pos+1,0);
					memset(ch,0,sizeof(ch));
					pos++;
					SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_GETTEXT,(WPARAM)pos,(LPARAM)ch);
					for(i=0;i<strlen(ch)&&ch[i]!='-';i++)i=i;
					memset(chr,0,sizeof(chr));
					strcpy(chr,ch+i+3);
					memset(ch+i+3,0,1);
					SetWindowTextA(GetDlgItem(hwd,IDD2_4E),ch);
					SetWindowTextA(GetDlgItem(hwd,IDD_5E),chr);
				}
				break;
			}

			if(wp==IDD_4B)
			{
				
				if(pos>0)
				{
					SendMessage(GetDlgItem(hwd,IDD_LS1),LB_SETCURSEL,(WPARAM)pos-1,0);
					memset(ch,0,sizeof(ch));
					pos--;
					SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_GETTEXT,(WPARAM)pos,(LPARAM)ch);
					for(i=0;i<strlen(ch)&&ch[i]!='-';i++)i=i;
					memset(chr,0,sizeof(chr));
					strcpy(chr,ch+i+3);
					memset(ch+i+3,0,1);
					SetWindowTextA(GetDlgItem(hwd,IDD2_4E),ch);
					SetWindowTextA(GetDlgItem(hwd,IDD_5E),chr);
				}
				break;
			}

			if(wp==IDD_6B)
			{
				if(MessageBox(hwd,L"Czy napewno chcesz anulowac ?",L"ANULUJ",MB_YESNO)==IDNO)break;
				else
				{
					DestroyWindow(Dlg2);
					DestroyWindow(hwd);
					break;
				}
				break;
			}
			if(wp==IDD_5B)
			{
				for(pos=0;pos<wc;pos++)
				{
					memset(ch,0,sizeof(ch));
					SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_GETTEXT,(WPARAM)pos,(LPARAM)ch);
					for(i=0;i<strlen(ch)&&ch[i]!='-';i++)i=i;
					if(ch[i+3]==0)
					{
						MessageBox(hwd,L"Musisz wypelnic wszystkie opisy",L"Info",MB_OK);break;
					}
				}
				if(pos==wc)
				{
					GetWindowRect(hwd,&rec);
					ShowWindow(hwd,SW_HIDE);
					ShowWindow(Dlg2,SW_SHOW);
					MoveWindow(Dlg2,rec.left,rec.top,rec.right-rec.left,rec.bottom-rec.top,true);
					break;
				}
				break;
			}
			if(LOWORD(wp)==IDD_LS1)
			{
				if(HIWORD(wp)==LBN_SELCHANGE)
				{
					memset(ch,0,sizeof(ch));
					pos=ListBox_GetCurSel(GetDlgItem(hwd,IDD_LS1));
					SendMessageA(GetDlgItem(hwd,IDD_LS1),LB_GETTEXT,(WPARAM)pos,(LPARAM)ch);
					for(i=0;i<strlen(ch)&&ch[i]!='-';i++)i=i;
					memset(chr,0,sizeof(chr));
					strcpy(chr,ch+i+3);
					memset(ch+i+3,0,1);
					SetWindowTextA(GetDlgItem(hwd,IDD2_4E),ch);
					SetWindowTextA(GetDlgItem(hwd,IDD_5E),chr);
					break;
				}
			}
			break;
		}

	case WM_CLOSE:
		{
			if(MessageBox(hwd,L"Czy napewno chcesz anulowac ?",L"ANULUJ",MB_YESNO)==IDNO)break;
			DestroyWindow(Dlg2);
			DestroyWindow(hwd);
			break;
		}
	case WM_DESTROY:
		{
			EnableWindow(M_Wnd,true);
			//PostQuitMessage(0);

		}

	default:
		{
			return DefDlgProc(M_Wnd,code,wp,lp);
		}
	}

return 0;

}



	//Procedura opisu okna 2 dialogowego opisu wartosci



INT_PTR CALLBACK Dlg2_Init(HWND hwd,UINT code, WPARAM wp, LPARAM lp)
{
	char naz[125];
	char op[125];
	wchar_t st[150];
	static int pos,count;
	int i;
	static bool bl =false;
	switch (code)
	{
	case WM_INITDIALOG:
		{
			//ShowWindow(hwd,SW_NORMAL);
			SetWindowText(GetDlgItem(hwd,IDD_6E),L"Y1-");
			Dlg2=hwd;
			pos=0;
			count=yc;
			for(i=0;i<yc;i++)
			{
				memset(naz,0,sizeof(naz));
				naz[0]='Y';
				itoa(i+1,naz+1,10);
				strcpy(naz+strlen(naz),"-");
				SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_ADDSTRING,0,(LPARAM)naz);
			}
			SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_SETCURSEL,(WPARAM)0,0);
			break;
		}

	case WM_COMMAND:
		{
			if(wp==IDD_7B)
			{
				if(count==pos)
				{
					MessageBox(hwd,L"Podales juz maksymalna ilosc wysjc",L"Informacja",MB_OK);
					break;
				}
				//if(pos+1>count) break;
				memset(op,0,sizeof(op));
				GetWindowTextA(GetDlgItem(hwd,IDD_7E),op,sizeof(op));
				if(op[0]==0)
				{
					MessageBox(hwd,L"Brak opisu wartosci wyjsciowej",L"INFO",MB_OK);
					break;
				}
			
				memset(naz,0,sizeof(naz));
				GetWindowTextA(GetDlgItem(hwd,IDD_6E),naz,sizeof(naz));
				strcpy(naz+strlen(naz),op);
				SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_DELETESTRING,(WPARAM)pos,(LPARAM)naz);
				SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_INSERTSTRING,(WPARAM)pos,(LPARAM)naz);
				if(pos+1==count)SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_SETCURSEL,(WPARAM)pos,0); else
				SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_SETCURSEL,(WPARAM)pos+1,0);
				if(pos+1==count) bl=true;
				if(pos+1<count)
				{
					pos++;
					naz[0]='Y';
					itoa(pos+1,naz+1,10);
					strcpy(naz+strlen(naz),"-");
					SetWindowTextA(GetDlgItem(hwd,IDD_6E),naz);
					memset(naz,0,sizeof(naz));
					SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_GETTEXT,(WPARAM)pos,(LPARAM)naz);
					for(i=0;i<strlen(naz)&&naz[i]!='-';i++)i=i;
					SetWindowTextA(GetDlgItem(hwd,IDD_7E),naz+i+1);
				}
				break;
			}
		
			if(wp==IDD_8B)
			{
				if(pos==0)break;
				pos--;
				SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_SETCURSEL,(WPARAM)pos,0);
				memset(naz,0,sizeof(naz));
				naz[0]='Y';
				itoa(pos+1,naz+1,10);
				strcpy(naz+strlen(naz),"-");
				SetWindowTextA(GetDlgItem(hwd,IDD_6E),naz);
				memset(naz,0,sizeof(naz));
				SendMessageA(GetDlgItem(hwd,IDD_LS2),LB_GETTEXT,(WPARAM)pos,(LPARAM)naz);
				for(i=0;i<strlen(naz)&&naz[i]!='-';i++)i=i;
				SetWindowTextA(GetDlgItem(hwd,IDD_7E),naz+i+1);
			}
			if(wp==IDD_9B)
			{
				if(!bl)
				{
					MessageBox(hwd,L"Musisz podac wszystkie opisy dla atrybutow wyjsciowych",L"INFO",MB_OK);
					break;
				}
				CPPH.Cpin_count=wc;
				CPPH.Cpout_count=count;
				CPI=new CP_IN[wc];
				CPO=new CP_OUT[count];
				i=Save_CPIO(CPPH,CPI,CPO,GetDlgItem(Dlg1,IDD_LS1),GetDlgItem(hwd,IDD_LS2));
				if(i==-1)MessageBox(hwd,L"Nie mozna utworzyc pliku bazy",L"ERROR",MB_OK);
				else if(i==0)MessageBox(hwd,L"Blad w zapisie parametrow",L"ERROR",MB_OK);
				else if(i==1)
				{
					MessageBox(hwd,L"Parametry zostaly zapisane",L"KOMUNIKAT",MB_OK);
					param=true;
					EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),1),3),MF_ENABLED);
					if(CPI!=NULL)
					{
						SendMessage(Lst1,LB_RESETCONTENT,0,0);
						for(i=0;i<CPPH.Cpin_count;i++)
						{
							memset(st,0,sizeof(st));
							wcscpy(st,L"X");
							_itow(i+1,st+1,10);
							wcscpy(st+wcslen(st),L"-");
							wcscpy(st+wcslen(st),CPI[i].opis);
							wcscpy(st+wcslen(st),L"=");
							SendMessage(Lst1,LB_ADDSTRING,0,(LPARAM)st);
						}
						SetWindowTextA(St1,"Parametr X1");
						SendMessage(Lst1,LB_SETCURSEL,0,0);
					}

					if(CPO!=NULL)
					{
						SendMessage(Lst2,LB_RESETCONTENT,0,0);
						for(i=0;i<CPPH.Cpout_count;i++)
						{
							memset(st,0,sizeof(st));
							wcscpy(st,L"Y");
							_itow(i+1,st+1,10);
							wcscpy(st+wcslen(st),L"-");
							wcscpy(st+wcslen(st),CPO[i].opis);
							SendMessage(Lst2,LB_ADDSTRING,0,(LPARAM)st);
						}
					}
					memset(input,-2,sizeof(input));
					DestroyWindow(Dlg1);
					DestroyWindow(hwd);
				}
				break;
			}
			if(wp==IDD_10B)
			{
				if(MessageBox(hwd,L"Czy napewno chcesz anulowac ?",L"ANULUJ",MB_YESNO)==IDNO)break;
				else
				{
					DestroyWindow(Dlg1);
					DestroyWindow(hwd);
					break;
				}
			}
			break;
		}
	case WM_CLOSE:
		{
			if(MessageBox(hwd,L"Czy napewno chcesz anulowac ?",L"ANULUJ",MB_YESNO)==IDNO)break;
			DestroyWindow(Dlg1);
			DestroyWindow(hwd);
			break;
		}
	case WM_DESTROY:
		{
			EnableWindow(M_Wnd,true);
			//PostQuitMessage(0);

		}


	default:
		{
			return DefDlgProc(M_Wnd,code,wp,lp);
		}
	}

return 0;
	
}





INT_PTR CALLBACK Dlg_Init3(HWND hwd,UINT code, WPARAM wp, LPARAM lp)
{
	int i,i1;
	static int pos,count,pos1;
	wchar_t ch[150];
	wchar_t ch2[150];
	static float *WX,f;
	char ch1[10];
	bool zap=true;
	CP_TEACHER *CPT1;
	
	switch (code)
	{
	case WM_INITDIALOG:
		{
			if(CPI==NULL||CPO==NULL)
			{
				MessageBox(hwd,L"Nie mozna odczytac danych na temat parametrow wejsciowych i wyjsciowych",L"ERROR",MB_OK);
				DestroyWindow(hwd);
				break;
			}
			SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_RESETCONTENT,0,0);
			SendMessage(GetDlgItem(hwd,IDD4_LS1),LB_RESETCONTENT,0,0);
			SendMessage(GetDlgItem(hwd,IDD4_CB1),CB_RESETCONTENT,0,0);
			pos=0;
			count=0;
			pos1=teach_c;
			WX=new float[CPPH.Cpin_count];
			for(i=0;i<CPPH.Cpin_count;i++)
			{
				WX[i]=-2;
				memset(ch,0,sizeof(ch));
				ch[0]='X';
				_itow(i+1,ch+1,10);
				wcscpy(ch+ wcslen(ch),L"-");
				wcscpy(ch + wcslen(ch),CPI[i].opis);
				wcscpy(ch + wcslen(ch),L"=");
				SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_ADDSTRING,0,(LPARAM)ch);
				SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_SETCURSEL,0,0);
				count++;
				if(teach_c!=0)
				{
					gcvt(CPT[teach_c-1].WX[i],2,ch1);
					for(i1=0;i1<strlen(ch1);i1++) mbtowc(ch+wcslen(ch),ch1+i1,1);
					if(ch[wcslen(ch)-1]=='.')ch[wcslen(ch)-1]=0;
					SendMessage(GetDlgItem(hwd,IDD4_LS1),LB_ADDSTRING,0,(LPARAM)ch);
				}
			}
			if(teach_c!=0)
			{
				memset(ch,0,sizeof(ch));
				ch[0]='Y';
				for(i=0;i<CPPH.Cpout_count;i++)
				{
					if(CPT[pos1-1].WY[i]==1)break;
				}
				_itow(i+1,ch+1,10);
				wcscpy(ch+ wcslen(ch),L"-");
				wcscpy(ch + wcslen(ch),CPO[i].opis);
				SetWindowText(GetDlgItem(hwd,IDD4_E3),ch);
				
			}
			for(i=0;i<CPPH.Cpout_count;i++)
			{
				memset(ch,0,sizeof(ch));
				ch[0]='Y';
				_itow(i+1,ch+1,10);
				wcscpy(ch+ wcslen(ch),L"-");
				wcscpy(ch + wcslen(ch),CPO[i].opis);
				SendMessage(GetDlgItem(hwd,IDD4_CB1),CB_ADDSTRING,0,(LPARAM)ch);
				SendMessage(GetDlgItem(hwd,IDD4_CB1),CB_SETCURSEL,0,0);
			}
			SetWindowText(GetDlgItem(hwd,IDD4_E1),L"X1");
			memset(ch,0,sizeof(ch));
			wcscpy(ch,L"Rekord ");
			_itow(pos1,ch+wcslen(ch),10);
			wcscpy(ch+wcslen(ch),L" z ");
			_itow(teach_c,ch+wcslen(ch),10);
			SetWindowText(GetDlgItem(hwd,IDD4_T3),ch);
			ShowWindow(hwd,SW_NORMAL);

			break;
		}

	case WM_COMMAND:
		{
			if(LOWORD(wp)==IDD4_LS2)
			{
				if(HIWORD(wp)==LBN_SELCHANGE)
				{
					pos=SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_GETCURSEL,0,0);
					memset(ch,0,sizeof(ch));
					ch[0]='X';
					_itow(pos+1,ch+1,10);
					SetWindowText(GetDlgItem(hwd,IDD4_E1),ch);
					if(WX[pos]!=-2)
					{
						memset(ch1,0,sizeof(ch1));
						gcvt(WX[pos],2,ch1);
						SetWindowTextA(GetDlgItem(hwd,IDD4_E2),ch1);
					}
					break;
				}
				break;
			}
			if(wp==IDD4_B1)		//Przypisz
			{
				memset(ch,0,sizeof(ch));
				GetWindowText(GetDlgItem(hwd,IDD4_E2),ch,sizeof(ch));
				f=-2;
				if((ch[0]=='0')&&(ch[1]==0)) f=0;

				else
				{
					f=(float)_wtof(ch);
					if(f==0)
			
					{
						MessageBox(hwd,L"Nieprawidlowa wartosc zmienoprzecinkowa\nPamietaj zeby oddzielic czesc calkowira od ulamkowej kropka",L"Nieprawidlowy argument",MB_OK);
						break;
					}
					if(f>1||f<0)
			
					{
						MessageBox(hwd,L"Musisz podac liczbe z zakresu (0 do 1)",L"Nieprawidlowy argument",MB_OK);
						break;
					}
				}
				WX[pos]=f;
				memset(ch2,0,sizeof(ch2));
				ch2[0]='X';
				_itow(pos+1,ch2+1,10);
				wcscpy(ch2+ wcslen(ch2),L"-");
				wcscpy(ch2 + wcslen(ch2),CPI[pos].opis);
				wcscpy(ch2 + wcslen(ch2),L"=");
				wcscpy(ch2+wcslen(ch2),ch);
				SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_DELETESTRING,pos,0);
				SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_INSERTSTRING,pos,(LPARAM)ch2);
				SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_SETCURSEL,pos,0);
				if(pos+1==count)break;
				pos++;
				wp=LBN_SELCHANGE;
				wp=wp<<16;
				wp=wp+IDD4_LS2;
				SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_SETCURSEL,pos,0);
				SendMessage(hwd,WM_COMMAND,wp,0);
				break;
			}

			if(wp==IDD4_B2)		//Poprzedni
			{
				if(pos==0)break;
				pos--;
				wp=LBN_SELCHANGE;
				wp=wp<<16;
				wp=wp+IDD4_LS2;
				SendMessage(GetDlgItem(hwd,IDD4_LS2),LB_SETCURSEL,pos,0);
				SendMessage(hwd,WM_COMMAND,wp,0);
				break;
			}

			if(wp==IDD4_B5)								//Dodaj
			{
				if(teach_c==MAX_T)
				{
					MessageBox(hwd,L"Podales juz maksymalna ilosc skladowych ciagu uczacego\nUruchom proces uczenia sieci lub w celu utworzenia nowego ciagu\nW menu aplikacji wybierz Skasuj istniejacy ciag",L"Komunikat",MB_OK);
					break;
				}
				for(i=0;i<count;i++)
				{
					if(WX[i]==-2)
					{
						MessageBox(hwd,L"Musisz podac parametry dla wszystkich wartosci wejsciowych",L"Komunikat",MB_OK);
						zap=false;
						break;
					}
				}
				if(!zap) break;
				CPT[teach_c].count_x=count;
				CPT[teach_c].count_y=CPPH.Cpout_count;
				for(i=0;i<count;i++)
				{
					CPT[teach_c].WX[i]=WX[i];
				}
				i1=SendMessage(GetDlgItem(hwd,IDD4_CB1),CB_GETCURSEL,0,0);
				for(i=0;i<CPPH.Cpout_count;i++)
				{
					if(i==i1) CPT[teach_c].WY[i]=1; else CPT[teach_c].WY[i]=0;
				}
				teach_c++;
				zap_cu=false;
				SendMessage(hwd,WM_INITDIALOG,0,0);
				break;
			}
			if(wp==IDD4_B6)						//Usun
			{
				if(teach_c==0)break;
				teach_c--;
				CPT1 = new CP_TEACHER[teach_c];
				i1=0;
				for(i=0;i<teach_c+1;i++)
				{
					if(i!=pos1-1) 
					{
						CPT1[i1]=CPT[i];
						i1++;
					}
					
				}
				memset(CPT,0,sizeof(CP_TEACHER)*(teach_c+1));
				memcpy(CPT,CPT1,sizeof(CP_TEACHER)*teach_c);
				free(CPT1);
				SendMessage(hwd,WM_INITDIALOG,0,0);
				zap_cu=false;
				break;
			}

			if(wp==IDD4_B7)														//>>
			{
				if(pos1==teach_c) break;
				SendMessage(GetDlgItem(hwd,IDD4_LS1),LB_RESETCONTENT,0,0);
				pos1++;
				for(i=0;i<CPPH.Cpin_count;i++)
				{
					memset(ch,0,sizeof(ch));
					ch[0]='X';
					_itow(i+1,ch+1,10);
					wcscpy(ch+ wcslen(ch),L"-");
					wcscpy(ch + wcslen(ch),CPI[i].opis);
					wcscpy(ch + wcslen(ch),L"=");
					gcvt(CPT[pos1-1].WX[i],2,ch1);
					for(i1=0;i1<strlen(ch1);i1++) mbtowc(ch+wcslen(ch),ch1+i1,1);
					SendMessage(GetDlgItem(hwd,IDD4_LS1),LB_ADDSTRING,0,(LPARAM)ch);
				}
				memset(ch,0,sizeof(ch));
				ch[0]='Y';
				for(i=0;i<CPPH.Cpout_count;i++)
				{
					if(CPT[pos1-1].WY[i]==1)break;
				}
				_itow(i+1,ch+1,10);
				wcscpy(ch+ wcslen(ch),L"-");
				wcscpy(ch + wcslen(ch),CPO[i].opis);
				SetWindowText(GetDlgItem(hwd,IDD4_E3),ch);
				memset(ch,0,sizeof(ch));
				wcscpy(ch,L"Rekord ");
				_itow(pos1,ch+wcslen(ch),10);
				wcscpy(ch+wcslen(ch),L" z ");
				_itow(teach_c,ch+wcslen(ch),10);
				SetWindowText(GetDlgItem(hwd,IDD4_T3),ch);
			}
			if(wp==IDD4_B8)															//<<
			{
				if(pos1==1||pos1==0) break;
				SendMessage(GetDlgItem(hwd,IDD4_LS1),LB_RESETCONTENT,0,0);
				pos1--;
				for(i=0;i<CPPH.Cpin_count;i++)
				{
					memset(ch,0,sizeof(ch));
					ch[0]='X';
					_itow(i+1,ch+1,10);
					wcscpy(ch+ wcslen(ch),L"-");
					wcscpy(ch + wcslen(ch),CPI[i].opis);
					wcscpy(ch + wcslen(ch),L"=");
					gcvt(CPT[pos1-1].WX[i],2,ch1);
					for(i1=0;i1<strlen(ch1);i1++) mbtowc(ch+wcslen(ch),ch1+i1,1);
					SendMessage(GetDlgItem(hwd,IDD4_LS1),LB_ADDSTRING,0,(LPARAM)ch);
				}
				memset(ch,0,sizeof(ch));
				ch[0]='Y';
				for(i=0;i<CPPH.Cpout_count;i++)
				{
					if(CPT[pos1-1].WY[i]==1)break;
				}
				_itow(i+1,ch+1,10);
				wcscpy(ch+ wcslen(ch),L"-");
				wcscpy(ch + wcslen(ch),CPO[i].opis);
				SetWindowText(GetDlgItem(hwd,IDD4_E3),ch);
				memset(ch,0,sizeof(ch));
				wcscpy(ch,L"Rekord ");
				_itow(pos1,ch+wcslen(ch),10);
				wcscpy(ch+wcslen(ch),L" z ");
				_itow(teach_c,ch+wcslen(ch),10);
				SetWindowText(GetDlgItem(hwd,IDD4_T3),ch);
			}
			if(wp==IDD4_B3)						//Zapisz do pliku
			{
				Create_T(CPT,teach_c);
				zap_cu=true;
				MessageBox(hwd,L"Zapisano......",L"Info",MB_OK);
				break;
			}
			if(wp==IDD4_B4)
			{
				SendMessage(hwd,WM_CLOSE,0,0);
				break;
			}

			break;
		}

	case WM_CLOSE:
		{
			if(!zap_cu)
			{
				if(MessageBox(hwd,L"Ciag nie zostal zapisany do pliku\n Jesli wylaczysz okno to zmiany zostana utracone",L"INFO",MB_YESNO)==IDNO)break;
			}
			DestroyWindow(hwd);
			if(WX!=NULL)
				{
					delete WX;
			}
			break;
		}
	case WM_DESTROY:
		{
			Load_H(&HDN);
			teach_c=HDN.TEACHE_C;
			if(teach_c>0) EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),2),1),MF_ENABLED);else EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),2),1),MF_DISABLED);
			EnableWindow(M_Wnd,true);
			//PostQuitMessage(0);

		}

	default:
		{
			return DefDlgProc(M_Wnd,code,wp,lp);
		}
	}

return 0;
	
}