//---------------------------------------------------------------------------

#ifndef inheritance2H
#define inheritance2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
static int cm=0, cl=0, cs=0, cr=0, cp=0;
class employee
{
 private:
           static int count;
 protected:
           String name;
           String id;
 public:
           employee() {};
           employee *next;
           employee(String n, String i) : id(i), name(n) {next=NULL; count++; };
           String get_name() { return name; }
           String get_id() { return id; }
           int get_count() { return count; }
           ~employee() {count--;}
           virtual void disp(TListBox *ListBox1)=0;
};
int employee::count=0;

class manager : public employee
{
 private:
           String title;
           String age;
 public:
           manager() {};
           manager(String n, String i, String t, String a) : employee(n,i), title(t), age(a) {};
           String get_title() { return title; }
           String get_age() { return age; }
           void disp(TListBox *box)
           {
           box->Items->Add(name+", "+id+", "+title+", "+age);
           };
};

class scientist : public employee
{
 protected:
           String publications;
 public:
           scientist() {};
           scientist(String n, String i, String p) : employee(n,i), publications(p) {};
};

class laborer : public employee
{
 private:
 public:
           laborer() {};
           laborer(String n, String i) : employee(n,i) {};
           void disp(TListBox *box)
           {
           box->Items->Add(name+", "+id);
           };
};

class professor : public scientist
{
 private:
 public:
           professor() {};
           professor(String n, String i, String p) : scientist(n,i,p) {};
           void disp(TListBox *box)
           {
           box->Items->Add(name+", "+id+", "+publications);
           };
           String get_pub() { return publications; }
};

class student : public scientist
{
 private:
 public:
           student() {};
           student(String n, String i, String p) : scientist(n,i,p) {};
           void disp(TListBox *ListBox1)
           {
           ListBox1->Items->Add(name+", "+id+", "+publications);
           };
           String get_pub() { return publications; }
};

class researcher : public scientist
{
 private:
 public:
           researcher() {};
           researcher(String n, String i, String p) : scientist(n,i,p) {};
           void disp(TListBox *box)
           {
           box->Items->Add(name+", "+id+", "+publications);
           };
           String get_pub() { return publications; }
};
//---------------------------------------------------------------------------
class list
{
private:
        employee *man1;
        employee *lab1;
        employee *prof1;
        employee *stu1;
        employee *res1;
public:
        ~list() {};
        list() { man1=NULL; lab1=NULL; prof1=NULL; stu1=NULL; res1=NULL; };
        void add_m(String n, String i, String t, String a);
        void add_l(String n, String i);
        void add_s(int type, String n, String i, String p);
        void disp_m(TListBox *ListBox1 ,int type);
        void disp_count(TEdit *Edit16);
        void search(TListBox *ListBox2, int type, String fraze);
        void del(int type, int index);
        void edit(TEdit *Edit18, TEdit *Edit19, TEdit *Edit20, TEdit *Edit21, TEdit *Edit22,int type, int index);

};

void list::edit(TEdit *Edit18, TEdit *Edit19, TEdit *Edit20, TEdit *Edit21, TEdit *Edit22, int type, int index)
{
employee *tmp;
employee *prev;
employee *edit;
employee *new_m;
employee *new_l;
employee *new_s;
employee *new_r;
employee *new_p;
String name=Edit18->Text;
String id=Edit19->Text;
String title=Edit20->Text;
String age=Edit21->Text;
String pub=Edit22->Text;
int counter=0, del;
if(type==1) { prev=man1; edit=new manager(name, id, title,age); }
if(type==2) { prev=lab1; edit=new laborer(name, id);}
if(type==3) { prev=stu1; edit=new student(name, id, pub);}
if(type==4) { prev=res1; edit=new researcher(name, id, pub);}
if(type==5) { prev=prof1; edit=new professor(name, id, pub);}

if(prev==NULL) { return; }
else
{
if(type==1) { tmp=man1;} if(type==2) { tmp=lab1;} if(type==3) { tmp=stu1;}
if(type==4) { tmp=res1;} if(type==5) { tmp=prof1;}

while(index!=counter)
{
tmp=tmp->next;
tmp=prev->next;
counter++;
}
edit->next=tmp->next;
prev->next=edit;

}

}

void list::del(int type, int index)
{
employee *tmp;
employee *prev;
int counter=0, del;
if(type==1) { prev=man1;}
if(type==2) { prev=lab1;}
if(type==3) { prev=stu1;}
if(type==4) { prev=res1;}
if(type==5) { prev=prof1;}

if(prev==NULL) { return; }
else if(prev->next==NULL && index==counter)
{
if(type==1) { man1=NULL; cm--;} if(type==2) { lab1=NULL; cl--;} if(type==3) { stu1=NULL; cs--;}
if(type==4) { res1=NULL; cr--;} if(type==5) { prof1=NULL; cp--;}
}
else if(prev->next!=NULL && index==counter)
{
tmp=prev->next;
delete prev;
if(type==1) { man1=tmp; cm--;} if(type==2) { lab1=tmp; cl--;}if(type==3) { stu1=tmp;cs--;}
if(type==4) { res1=tmp; cr--;} if(type==5) { prof1=tmp; cp--;}
}
else
{
if(type==1) { tmp=man1; cm--;} if(type==2) { tmp=lab1; cl--;} if(type==3) { tmp=stu1; cs--;}
if(type==4) { tmp=res1; cr--;} if(type==5) { tmp=prof1; cp--;}
while(index!=counter)
{
tmp=tmp->next;
tmp=prev->next;
counter++;
}
prev->next=tmp->next;
delete tmp;


}

}

void list::search(TListBox *ListBox2, int type, String fraze)
{
employee *tmp;
String name, id;
if(type==1) { tmp=man1; }
if(type==2) { tmp=lab1; }
if(type==3) { tmp=stu1; }
if(type==4) { tmp=res1; }
if(type==5) { tmp=prof1; }

if(tmp!=NULL)
{
do{
if(tmp->get_name()==fraze || tmp->get_id()==fraze)
{
tmp->disp(ListBox2);
}
tmp=tmp->next;
}while(tmp!=NULL);
}

}

void list::disp_count(TEdit *Edit16)
{
employee *tmp;
tmp=new student;

Edit16->Clear();
Edit16->Text=tmp->get_count();
}

void list::add_m(String n, String i, String t, String a)
{
employee *new_manager;
employee *tmp=man1;

new_manager = new manager(n,i,t,a);

new_manager->next=tmp; man1=new_manager;
cm++;
}

void list::add_l(String n, String i)
{
employee *new_l;
employee *tmp=lab1;
new_l = new laborer(n,i);
cl++;
new_l->next=tmp; lab1=new_l;
}

void list::add_s(int type, String n, String i, String p)
{
employee *new_s;
employee *new_r;
employee *new_p;
employee *tmp;

if(type==3) { new_s = new student(n,i,p); tmp=stu1; new_s->next=tmp; stu1=new_s; cs++;}
if(type==4) { new_r = new researcher(n,i,p); tmp=res1; new_r->next=tmp; res1=new_r; cr++;}
if(type==5) { new_p = new professor(n,i,p); tmp=prof1; new_p->next=tmp; prof1=new_p; cp++;}

}

void list::disp_m(TListBox *ListBox1 ,int type)
{

employee *tmp_m;
employee *tmp_l;
employee *tmp_s;
employee *tmp_r;
employee *tmp_p;

if(type==1 || type==0) { tmp_m=man1; }
if(type==2 || type==0) { tmp_l=lab1; }
if(type==3 || type==0) { tmp_s=stu1; }
if(type==4 || type==0) { tmp_r=res1; }
if(type==5 || type==0) { tmp_p=prof1; }

if((type==1 || type==0) && tmp_m!=NULL)
{
do{
tmp_m->disp(ListBox1); tmp_m=tmp_m->next;
}while(tmp_m!=NULL);
}

if((type==2 || type==0) && tmp_l!=NULL)
{
do{
tmp_l->disp(ListBox1); tmp_l=tmp_l->next;
}while(tmp_l!=NULL);
}

if((type==3 || type==0) && tmp_s!=NULL)
{
do{
tmp_s->disp(ListBox1); tmp_s=tmp_s->next;
}while(tmp_s!=NULL);
}

if((type==4 || type==0) && tmp_r!=NULL)
{
do{
tmp_r->disp(ListBox1); tmp_r=tmp_r->next;
}while(tmp_r!=NULL);
}

if((type==5 || type==0) && tmp_p!=NULL)
{
do{
tmp_p->disp(ListBox1); tmp_p=tmp_p->next;
}while(tmp_p!=NULL);
}

}
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TListBox *ListBox1;
        TComboBox *ComboBox1;
        TGroupBox *GroupBox1;
        TEdit *Edit1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *Edit2;
        TLabel *Label4;
        TEdit *Edit3;
        TGroupBox *GroupBox2;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *Edit4;
        TEdit *Edit5;
        TGroupBox *GroupBox3;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *Edit6;
        TEdit *Edit7;
        TEdit *Edit8;
        TGroupBox *GroupBox4;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *Edit9;
        TEdit *Edit10;
        TEdit *Edit11;
        TGroupBox *GroupBox5;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TEdit *Edit12;
        TEdit *Edit13;
        TEdit *Edit14;
        TLabel *Label16;
        TEdit *Edit15;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TEdit *Edit16;
        TLabel *Label17;
        TGroupBox *GroupBox6;
        TEdit *Edit17;
        TLabel *Label18;
        TComboBox *ComboBox2;
        TListBox *ListBox2;
        TLabel *Label19;
        TLabel *Label20;
        TButton *Button6;
        TGroupBox *GroupBox7;
        TButton *Button7;
        TButton *Button8;
        TGroupBox *GroupBox8;
        TLabel *Label1;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TButton *Button9;
        TEdit *Edit22;
        TEdit *Edit21;
        TEdit *Edit20;
        TEdit *Edit19;
        TEdit *Edit18;
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
private:	// User declarations
public:
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
