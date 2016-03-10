//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "inheritance2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
list empl;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
ComboBox1->Items->Add("ALL"); ComboBox1->Items->Add("MANAGERS"); ComboBox1->Items->Add("LABORERS");
ComboBox1->Items->Add("STUDENTS"); ComboBox1->Items->Add("RESEARCHERS"); ComboBox1->Items->Add("PROFESSORS");
ComboBox2->Items->Add("MANAGERS"); ComboBox2->Items->Add("LABORERS");
ComboBox2->Items->Add("STUDENTS"); ComboBox2->Items->Add("RESEARCHERS"); ComboBox2->Items->Add("PROFESSORS");
empl.add_m("Manager1","11151","Team2 Manager", "35");
empl.add_s(3, "Student1", "11", "5");
empl.add_s(3, "Student2", "11", "5");
empl.add_s(3, "Student3", "11", "5");
empl.add_s(3, "Student4", "11", "5");
empl.add_s(4, "Researcher1", "121", "11");
empl.add_s(5, "Professor1", "1131", "52");
empl.add_l("Laborer1", "11112");
empl.disp_m(ListBox1, 0);
empl.disp_count(Edit16);
ComboBox1->ItemIndex=0;
ComboBox1->ItemIndex=0;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
int type=0;
ListBox1->Clear();
empl.add_m(Edit12->Text, Edit13->Text, Edit14->Text, Edit15->Text);
empl.disp_m(ListBox1, type);
empl.disp_count(Edit16);
ComboBox1->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
int type=0;
ListBox1->Clear();
empl.add_l(Edit4->Text, Edit5->Text);
empl.disp_m(ListBox1, type);
empl.disp_count(Edit16);
ComboBox1->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
int type=0;
ListBox1->Clear();
empl.add_s(5, Edit9->Text, Edit10->Text, Edit11->Text);
empl.disp_m(ListBox1, type);
empl.disp_count(Edit16);
ComboBox1->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
int type=0;
ListBox1->Clear();
empl.add_s(4, Edit6->Text, Edit7->Text, Edit8->Text);
empl.disp_m(ListBox1, type);
empl.disp_count(Edit16);
ComboBox1->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
int type=0;
ListBox1->Clear();
empl.add_s(3, Edit1->Text, Edit2->Text, Edit3->Text);
empl.disp_m(ListBox1, type);
empl.disp_count(Edit16);
ComboBox1->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
int index;
index=ComboBox1->ItemIndex;
ListBox1->Clear();
empl.disp_m(ListBox1, index);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
{
int index;
index=ComboBox2->ItemIndex+1;
ListBox2->Clear();
empl.search(ListBox2, index, Edit17->Text);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button7Click(TObject *Sender)
{
int type=0, index=0, dtype=0;
if(ComboBox1->ItemIndex==0)
{
if(ListBox1->ItemIndex+1<=cm) { type=1; index=ListBox1->ItemIndex; }
else if(ListBox1->ItemIndex+1<=cl+cm) {type=2; index=ListBox1->ItemIndex-cm; }
else if(ListBox1->ItemIndex+1<=cs+cl+cm) {type=3; index=ListBox1->ItemIndex-cm-cl; }
else if(ListBox1->ItemIndex+1<=cr+cs+cl+cm) {type=4; index=ListBox1->ItemIndex-cm-cl-cs; }
else if(ListBox1->ItemIndex+1<=cp+cr+cs+cl+cm) {type=5; index=ListBox1->ItemIndex-cm-cl-cs-cr; }
}
else if(ComboBox1->ItemIndex==1) { type=1; dtype=1; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==2) { type=2; dtype=2; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==3) { type=3; dtype=3; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==4) { type=4; dtype=4; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==5) { type=5; dtype=5; index=ListBox1->ItemIndex;}

empl.del(type, index);
ListBox1->Clear();
empl.disp_m(ListBox1,dtype);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
GroupBox8->Visible=true;
Edit18->Visible=true; Edit19->Visible=true; Edit20->Visible=true;
Edit21->Visible=true; Edit22->Visible=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button9Click(TObject *Sender)
{
int type=0, index=0, dtype=0;
if(ComboBox1->ItemIndex==0)
{
if(ListBox1->ItemIndex+1<=cm) { type=1; index=ListBox1->ItemIndex; }
else if(ListBox1->ItemIndex+1<=cl+cm) {type=2; index=ListBox1->ItemIndex-cm; }
else if(ListBox1->ItemIndex+1<=cs+cl+cm) {type=3; index=ListBox1->ItemIndex-cm-cl; }
else if(ListBox1->ItemIndex+1<=cr+cs+cl+cm) {type=4; index=ListBox1->ItemIndex-cm-cl-cs; }
else if(ListBox1->ItemIndex+1<=cp+cr+cs+cl+cm) {type=5; index=ListBox1->ItemIndex-cm-cl-cs-cr; }
}
else if(ComboBox1->ItemIndex==1) { type=1; dtype=1; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==2) { type=2; dtype=2; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==3) { type=3; dtype=3; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==4) { type=4; dtype=4; index=ListBox1->ItemIndex;}
else if(ComboBox1->ItemIndex==5) { type=5; dtype=5; index=ListBox1->ItemIndex;}

empl.edit(Edit18, Edit19, Edit20, Edit21, Edit22, type, index);
ListBox1->Clear();
empl.disp_m(ListBox1,dtype);

GroupBox8->Visible=false;
Edit18->Visible=false; Edit19->Visible=false; Edit20->Visible=false;
Edit21->Visible=false; Edit22->Visible=false;
}
//---------------------------------------------------------------------------

