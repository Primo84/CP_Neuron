
#pragma comment(lib,"comctl32.lib")

#include "CPN.h"



MSG msg;

HINSTANCE hst;


using namespace std;


LRESULT _stdcall WndProc(HWND hwd, int code, WPARAM wp, LPARAM lp)
{
int i,j,len;
wchar_t str[150];
wchar_t str1[150];
char ch[500];
float fl;
bool bl;
char *buf,*buf1;
SCROLLINFO sci;

GETTEXTLENGTHEX gtl={GTL_NUMCHARS,CP_ACP};

	switch(code)
	{


	case WM_COMMAND:
		{
			if(LOWORD(wp)==ID_L1)
			{
				if(HIWORD(wp)==LBN_SELCHANGE)								//Zdarzenie listy zawierajacej parametry wejdciowe
				{
					i=SendMessage(Lst1,LB_GETCOUNT,0,0);
					if(i=0)
					{
						SetWindowTextA(St1,"");
						break;
					}
					memset(str,0,sizeof(str));
					i=SendMessage(Lst1,LB_GETCURSEL,0,0);
					wcscpy(str,L"Parametr X");
					_itow(i+1,str+wcslen(str),10);
					SetWindowText(St1,str);
					break;
				}
				break;
			}

			if(wp==ID_Bt_P)												//Przycisk Przypisz
			{
				if(SendMessage(Lst1,LB_GETCOUNT,0,0)<=0) break;
				memset(str,0,sizeof(str));
				GetWindowText(ET1,str,sizeof(str));
				if(str[0]==0)break;
				i=SendMessage(Lst1,LB_GETCURSEL,0,0);
				if(str[0]=='0'&&str[1]==0) fl=0;
				else
				{
					fl=(float)_wtof(str);
					if(fl==0)
					{
						MessageBox(hwd,L"Nieprawidlowa wartosc liczby zmiennoprzecinkowej",L"Nieprawidlowe dane",MB_OK);
						break;
					}
					if(fl<0||fl>1)
					{
						MessageBox(hwd,L"Musisz podac liczbe z zakresu od 0 do 1",L"Nieprawidlowe dane",MB_OK);
						break;
					}
				}
				memset(str1,0,sizeof(str1));
				str1[0]='X';
				_itow(i+1,str1+1,10);
				wcscpy(str1+wcslen(str1),L"-");
				wcscpy(str1+wcslen(str1),CPI[i].opis);
				wcscpy(str1+wcslen(str1),L"=");
				wcscpy(str1+wcslen(str1),str);
				SendMessage(Lst1,LB_DELETESTRING,(WPARAM)i,0);	
				SendMessage(Lst1,LB_INSERTSTRING,(WPARAM)i,(LPARAM)str1);
				input[i]=fl;
				if(i+1==CPPH.Cpin_count)SendMessage(Lst1,LB_SETCURSEL,(WPARAM)i,0); else 
				{
					SendMessage(Lst1,LB_SETCURSEL,(WPARAM)i,0);
					SendMessage(hwd,WM_COMMAND,(WPARAM)ID_BtF,0);
				}
				break;
			}

			if(wp==ID_BtF)																//Przycisk >>
			{
				i=SendMessage(Lst1,LB_GETCURSEL,0,0);
				if(i+1==SendMessage(Lst1,LB_GETCOUNT,0,0)) break;
				SendMessage(Lst1,LB_SETCURSEL,i+1,0);
				wp=LBN_SELCHANGE;;
				wp=wp<<16;
				wp=wp+ID_L1;
				SendMessage(M_Wnd,WM_COMMAND,wp,lp);
				break;
			}

			if(wp==ID_Bte)						//Przycisk wykonaj
			{
				bl=true;
				for(i=0;i<wc;i++)
				{
					if(input[i]==-2)
					{
						bl=false;
						MessageBox(hwd,L"Musisz podac wszystkie parametry wejsciowe",L"Brak Parametrow",MB_OK);
						break;
					}

				}
				if(bl)
				{
					
					if((j=Generate(CPK,CPG,HDN,input))==-1)
					{
						delete output;
						break;
					}
					
					len=SendMessage(Lst1,LB_GETCOUNT,0,0);
					for(i=0;i<SendMessage(Lst1,LB_GETCOUNT,0,0);i++) len=len+SendMessage(Lst1,LB_GETTEXTLEN,(WPARAM)i,0);
					len=len+SendMessage(Re_Wnd,EM_GETTEXTLENGTHEX,(WPARAM)&gtl,0);
					buf=new char[len+500];
					memset(buf,0,sizeof(buf));
					GetWindowTextA(Re_Wnd,buf,len+500);
					strcpy(buf+strlen(buf),"\n***Analiza zadania na podstawie podanych parametrow :\n");
					for(i=0;i<SendMessage(Lst1,LB_GETCOUNT,0,0);i++)
					{
						SendMessageA(Lst1,LB_GETTEXT,(WPARAM)i,(LPARAM)buf+strlen(buf));
						strcpy(buf+strlen(buf),"\n");
					}
					strcpy(buf+strlen(buf),"Wygenerowany zestaw odpowiedzi to: \n");
					SendMessageA(Lst2,LB_GETTEXT,(WPARAM)j,(LPARAM)buf+strlen(buf));
					strcpy(buf+strlen(buf),"\n");
					strcpy(buf+strlen(buf),"***Analiza zadania zakonczona***\n");
					SetWindowTextA(Re_Wnd,buf);
					for(i=0;i<MAX_W;i++)input[i]=-2;
					SendMessage(Lst1,LB_RESETCONTENT,0,0);
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
					SendMessage(Lst1,LB_SETCURSEL,(WPARAM)0,0);
					i=Edit_GetLineCount(Re_Wnd);
					j=Edit_GetFirstVisibleLine(Re_Wnd);
					if(i-j>=12) j=j+12;
					i=i-j;
					Edit_Scroll(Re_Wnd,i,0);
					delete buf;
					delete output;
				}
				break;
			}

			if(wp==ID_Bt_clr)
			{
				SetWindowText(Re_Wnd,L"");	
				break;
			}

			if(wp==ID_BtR)														//Przycisk <<
			{
				i=SendMessage(Lst1,LB_GETCURSEL,0,0);
				if(i<=0) break;
				SendMessage(Lst1,LB_SETCURSEL,i-1,0);
				wp=LBN_SELCHANGE;;
				wp=wp<<16;
				wp=wp+ID_L1;
				SendMessage(M_Wnd,WM_COMMAND,wp,lp);
				break;
			}

			if(wp==IDM_AC)													//Menu - Przetwarzania ciagu uczecego
			{
				if(teach_c==0)
				{
					MessageBox(hwd,L"Nie ma zadnych pozycji w ciagu uczcacym\nMusisz najpierw utworzyc nowy ciag",L"Wiadomosc",MB_OK);
					break;
				}
				CPT1=new CP_TEACHER[HDN.TEACHE_C];
				memcpy(CPT1,CPT,sizeof(CP_TEACHER)*HDN.TEACHE_C);
				k=Teach_CP(CPK,CPG,CPT,HDN);
				if(k==1)
				{
					MessageBox(hwd,L"Nauczanie zakonczone powodzeniem",L"Informacja",MB_OK);
					len=500;
					buf=new char[len];
					memset(buf,0,sizeof(buf));
					strcpy(buf,"\n");
					strcpy(buf+strlen(buf),"***Analiza ciagu uczacego***\n\n");
					for(i=0;i<HDN.TEACHE_C;i++)
					{
						memset(ch,0,sizeof(ch));
						for(j=0;j<wc;j++)
						{
							SendMessageA(Lst1,LB_GETTEXT,(WPARAM)j,(LPARAM)ch+strlen(ch));
							gcvt((double)CPT1[i].WX[j],5,ch+strlen(ch));
							if(ch[strlen(ch)-1]=='.') ch[strlen(ch)-1]=0;
							strcpy(ch+strlen(ch),"\n");
						}
						//memset(ch,0,sizeof(ch));
						for(j=0;j<CPT[j].count_y;j++)
						{
							if(CPT1[i].WY[j]==1)
								break;
						}
						SendMessageA(Lst2,LB_GETTEXT,(WPARAM)j,(LPARAM)ch+strlen(ch));
						if(i+1<HDN.TEACHE_C) strcpy(ch+strlen(ch),"\n----------------------------------------\n");

						if(strlen(ch)+strlen(buf)>=len)
						{
							len=len+strlen(ch)+500;
							buf1 = new char[len];
							memset(buf1,0,sizeof(buf1));
							strcpy(buf1,buf);
							delete buf;
							buf= new char[len];
							memset(buf,0,sizeof(buf));
							strcpy(buf,buf1);
							delete buf1;
						}
						strcpy(buf+strlen(buf),ch);

					}
					len=len+SendMessage(Re_Wnd,EM_GETTEXTLENGTHEX,(WPARAM)&gtl,0);
					len=len+50;
					buf1=new char[len];
					GetWindowTextA(Re_Wnd,buf1,len);
					strcpy(buf1+strlen(buf1),"\n");
					strcpy(buf1+strlen(buf1),buf);
					strcpy(buf1+strlen(buf1),"\n\n***Koniec analizy ciagu uczacego***\n");
					SetWindowTextA(Re_Wnd,buf1);
					i=Edit_GetLineCount(Re_Wnd);
					j=Edit_GetFirstVisibleLine(Re_Wnd);
					if(i-j>=12) j=j+12;
					i=i-j;
					Edit_Scroll(Re_Wnd,i,0);
					delete buf;
					delete buf1;

				} else
				{
					if(k==-1) MessageBox(hwd,L"Nie mozna zapisac zmian do pliku",L"Blad zapisu",MB_OK); else
						if(k==0) MessageBox(hwd,L"Wystapil nieznany blad",L"Blad",MB_OK);
				}
				free(CPT1);
				EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),2),1),MF_DISABLED);
				break;
			}

			if (wp==IDM_KONIEC)												//Menu - Zakoncz
			{
				DestroyWindow(hwd);
				break;
					
			}
			if(wp==IDM_KA)													//Menu - Konfiguracji atrybutow 
			{
				if(HDN.neuron_count==0)
				{
					MessageBox(M_Wnd,L"Musisz najpierw utworzyc siec neuronow",L"",MB_OK);
					break;
				}
				if(param)
				{
					if(MessageBox(M_Wnd,L"Atrybuty zostaly juz opisane.\nJesli zapiszesz nowe to stare zostana utracone\nCzy chcesz kontynulowac?",L"",MB_YESNO)==IDNO)break;
				}
				CreateDialog(hst,MAKEINTRESOURCE(IDD_2),M_Wnd,&Dlg1_Init);
				CreateDialog(hst,MAKEINTRESOURCE(IDD_3),M_Wnd,&Dlg2_Init);
				EnableWindow(M_Wnd,false);

			}
			if(wp==IDM_U_CU)														//Menu tworzenia ciagu uczacego
			{
				Load_H(&HDN);
				teach_c=HDN.TEACHE_C;
				Load_T(CPT);
				CreateDialog(hst,MAKEINTRESOURCE(IDD_4),M_Wnd,&Dlg_Init3);
				EnableWindow(M_Wnd,false);
			}
			if(wp==IDM_U_CP)														//Menu Tworzenia nowej sieci neuronow
			{
				if(MessageBox(M_Wnd,L"Jesli byla utworzona juz siec neuronow, to zostanie ona zastapiona \nCzy chcesz teraz utworzyc nowa siec ?",L"",MB_YESNO)==IDYES)
				{
					Init=false;
					CreateDialog(hst,MAKEINTRESOURCE(IDD_1),M_Wnd,&Dlg_Init);
					EnableWindow(M_Wnd,false);
			
				while(GetMessage(&msg,NULL,0,0))
				{
					if(!IsDialogMessage(NULL,&msg))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				
					if(Init)
					{
						CPK=new CP_NEURON[nc];
						CPG=new CP_NEURON[yc];
						Init_Cp(nc,ni,wc,yc,CPK,CPG);
						Init_Dll();
						Load_H(&HDN);
						EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),1),2),MF_ENABLED);
						DeleteFile(L"CP_In_Out.cpn");
						EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),1),3),MF_DISABLED);
						EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),2),1),MF_DISABLED);
						SendMessage(Lst1,LB_RESETCONTENT,0,0);
						SendMessage(Lst2,LB_RESETCONTENT,0,0);
						SetWindowTextA(St1,"Parametr---");
						param=false;
					}

				}
				

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
			if(CPK!=NULL) free(CPK);
			if(CPG!=NULL) free(CPG);
			if(CPI!=NULL)free (CPI);
			if(CPO!=NULL)free (CPO);
			PostQuitMessage(0);
			
		}
	
	default:
		{
			return DefWindowProc(hwd,code,wp,lp);
		}

	}

return 0;
}



int WINAPI WinMain(HINSTANCE hinst, HINSTANCE prev, LPSTR cmdstr, int cmdcode)
{
	
	HMODULE hmd,hmd1;
	char st[10];
	char st2[10]="Start";
	int i;

	hst=hinst;
	teach_c=0;
	for(i=0;i<MAX_W;i++)input[i]=-2;

	InitCommonControls();
	hmd1=LoadLibrary(L"riched20.dll");
	HDN.neuron_count=0;
	WNDCLASSEX Wcl={sizeof(WNDCLASSEX),CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS,(WNDPROC)WndProc,0,0,hinst,
					LoadIcon(0,IDI_APPLICATION),LoadIcon(hinst,MAKEINTRESOURCE(IDC_C1)),(HBRUSH)COLOR_WINDOW+3,NULL,L"MAINCLAS"};

	
	RegisterClassEx(&Wcl);

	

	M_Wnd=CreateWindowEx(WS_EX_OVERLAPPEDWINDOW |WS_EX_DLGMODALFRAME,L"MAINCLAS",L"CP_NEURON",WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX  | WS_CLIPCHILDREN,CW_USEDEFAULT,CW_USEDEFAULT,800,600,0,LoadMenu(hinst,MAKEINTRESOURCE(IDM_M1)),hinst,0);

	//WNetConnectionDialog(M_Wnd,0);
	CreateWindowMenu(hinst,M_Wnd);

	hmd =LoadLibraryA("Cp_Dll.dll");
	
	if(hmd==NULL) return -1;
	void (*pf1)(char* ch)=(void(*)(char*))GetProcAddress(hmd,"get_str");
	(*pf1)(st);

	FreeLibrary(hmd);

	i=strcmp(st,st2);		//zwraca 0 jesli ciagi sa rowne
	if(i!=0)  
	{
		Init=false;
		if(MessageBox(M_Wnd,L"Siec CP nie zostala jeszcze utworzona \nCzy chcesz teraz utworzyc",L"",MB_YESNO)==IDYES)
		{
			DeleteFile(L"CP_In_Out.cpn");
			CreateDialog(hinst,MAKEINTRESOURCE(IDD_1),M_Wnd,&Dlg_Init);
			EnableWindow(M_Wnd,false);
			
			while(GetMessage(&msg,NULL,0,0))
			{
				if(!IsDialogMessage(NULL,&msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

		}
		
		
		if(Init)
		{

			
			CPK=new CP_NEURON[nc];
			CPG=new CP_NEURON[yc];
			Init_Cp(nc,ni,wc,yc,CPK,CPG);
			Init_Dll();
			Load_H(&HDN);
			EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),1),2),MF_ENABLED);
		}
	} else
	{
		k=Load_H(&HDN);
		if(k==0)
		{
			MessageBox(M_Wnd,L"Nie mozna zaladowac pliku bazy",L"KOMUNIKAT",MB_OK);
		}
		else if(k==1)
		{
			nc=HDN.neuron_count;
			ni=HDN.n;
			teach_c=HDN.TEACHE_C;
			yc=HDN.yc;
			CPK= new CP_NEURON[nc];
			CPG= new CP_NEURON[yc];
			k=Load_Cp(CPK,CPG);
			if(teach_c>0)
			{
				k=Load_T(CPT); 
				if(k==0) MessageBox(M_Wnd,L"Nie mozna zaladowac ciagu uczacegu\nPowodem moze byc uszkodzony plik bazy",L"Error",MB_OK);
				else  EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),2),1),MF_ENABLED);
			}
			wc=CPK[0].count_w;
			//MessageBox(M_Wnd,L"Zaladowano pomyslnie",L"KOMUNIKAT",MB_OK);
			EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),1),2),MF_ENABLED);
			if(Load_CIO_Header(&CPPH)==1)
			{
				if(CPPH.Cpin_count>0&&CPPH.Cpout_count>0)
				{
					CPI=new CP_IN[CPPH.Cpin_count];
					CPO=new CP_OUT[CPPH.Cpout_count];
					if(Load_CIO(CPI,CPO)==1) EnableMenuItem(GetMenu(M_Wnd),GetMenuItemID(GetSubMenu(GetMenu(M_Wnd),1),3),MF_ENABLED);
					param=true;
				}
			}
		}

	}

	Init_ctlr(M_Wnd,hinst);

	UpdateWindow(M_Wnd);
	ShowWindow(M_Wnd,SW_NORMAL);

	while(GetMessage(&msg,NULL,0,0))
	{
		if(!IsDialogMessage(NULL,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}



return 0;

}

