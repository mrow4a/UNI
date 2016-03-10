//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "operator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
complex c1,c2,cr;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
enter(Edit1, Edit2, Edit3, Edit4, Memo1, Memo2, Memo3);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
enter(Edit1, Edit2, Edit3, Edit4, Memo1, Memo2, Memo3);


}
//---------------------------------------------------------------------------
void enter(TEdit *Edit1, TEdit *Edit2, TEdit *Edit3, TEdit *Edit4, TMemo *Memo1, TMemo *Memo2, TMemo *Memo3)
{
String c1r,c2r,c1i,c2i,crr,cri;
c1.set(Edit1->Text.ToDouble(),Edit2->Text.ToDouble());
c2.set(Edit3->Text.ToDouble(),Edit4->Text.ToDouble());
cr.set(0,0);
Memo1->Clear();
Memo2->Clear();
Memo3->Clear();
c1r=c1.get_r();
c2r=c2.get_r();
c1i=c1.get_i();
c2i=c2.get_i();
crr=cr.get_r();
cri=cr.get_i();
Memo1->Lines->Add("Re: "+c1r+" Im: "+c1i);
Memo2->Lines->Add("Re: "+c2r+" Im: "+c2i);
Memo3->Lines->Add("Re: "+crr+" Im: "+cri);
}
//---------------------------------------------------------------------------
void disp(TMemo *Memo1, TMemo *Memo2)
{
String c1r,c2r,c1i,c2i;

Memo1->Clear();
Memo2->Clear();
c1r=c1.get_r();
c2r=c2.get_r();
c1i=c1.get_i();
c2i=c2.get_i();
Memo1->Lines->Add("Re: "+c1r+" Im: "+c1i);
Memo2->Lines->Add("Re: "+c2r+" Im: "+c2i);
}
void result(TMemo *Memo3)
{
String crr,cri;

Memo3->Clear();
crr=cr.get_r();
cri=cr.get_i();

Memo3->Lines->Add("Re: "+crr+" Im: "+cri);

}
void __fastcall TForm1::Button2Click(TObject *Sender)
{
++c1;
++c2;
disp(Memo1, Memo2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
--c1;
--c2;
disp(Memo1, Memo2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
cr=c1+c2;
result(Memo3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
cr=c1-c2;
result(Memo3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
cr=c1*c2;
result(Memo3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
cr=c1/c2;
result(Memo3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
c1+=c2;
disp(Memo1, Memo2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
c1-=c2;
disp(Memo1, Memo2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
c1*=c2;
disp(Memo1, Memo2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender)
{
c1/=c2;
disp(Memo1, Memo2);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button12Click(TObject *Sender)
{
if(c1>c2==true) { Memo3->Clear(); Memo3->Lines->Add("TRUE"); }
else { Memo3->Clear(); Memo3->Lines->Add("FALSE"); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
if(c1<c2==true) { Memo3->Clear(); Memo3->Lines->Add("TRUE"); }
else { Memo3->Clear(); Memo3->Lines->Add("FALSE"); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button14Click(TObject *Sender)
{
if(c1>=c2==true) { Memo3->Clear(); Memo3->Lines->Add("TRUE"); }
else { Memo3->Clear(); Memo3->Lines->Add("FALSE"); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button15Click(TObject *Sender)
{
if(c1<=c2==true) { Memo3->Clear(); Memo3->Lines->Add("TRUE"); }
else { Memo3->Clear(); Memo3->Lines->Add("FALSE"); }
}
//---------------------------------------------------------------------------

