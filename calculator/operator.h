//---------------------------------------------------------------------------

#ifndef operatorH
#define operatorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------
class complex
{
      private:
              double r,i;
      public:
              complex(): r(0),i(0) {}
              complex(double rr, double ii): r(rr),i(ii){}
              double get_r() { return r; }
              void set(double rr, double ii) { r=rr; i=ii; } 
              double get_i() { return i; }
              complex operator ++()
              {
              ++r; return complex(r,i);
              }
              complex operator --()
              {
              --r; return complex(r,i);
              }
              complex operator +(complex c);
              complex operator -(complex c);
              complex operator *(complex c);
              complex operator /(complex c);

              complex operator +=(complex c2) { return complex(r+=c2.r,i+=c2.i); }
              complex operator -=(complex c2) { return complex(r-=c2.r,i-=c2.i); }
              complex operator *=(complex c2) { return complex(r*=c2.r,i*=c2.i); }
              complex operator /=(complex c2) { return complex(r/=c2.r,i/=c2.i); }

              bool operator >(complex c2) { return (i*i+r*r > c2.i * c2.i + c2.r * c2.r)?true:false; }
              bool operator <(complex c2) { return (i*i+r*r < c2.i * c2.i + c2.r * c2.r)?true:false; }
              bool operator >=(complex c2) { return (i*i+r*r >= c2.i * c2.i + c2.r * c2.r)?true:false; }
              bool operator <=(complex c2) { return (i*i+r*r <= c2.i * c2.i + c2.r * c2.r)?true:false; }
              


};
complex complex::operator +(complex c2)
{
return complex(r+c2.r,i+c2.i);
}
complex complex::operator -(complex c2)
{
return complex(r-c2.r,i-c2.i);
}
complex complex::operator *(complex c2)
{
return complex(r*c2.r,i*c2.i);
}
complex complex::operator /(complex c2)
{
return complex(r/c2.r,i/c2.i);
}

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit1;
        TLabel *Label1;
        TEdit *Edit2;
        TLabel *Label2;
        TEdit *Edit3;
        TEdit *Edit4;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TButton *Button1;
        TMemo *Memo1;
        TMemo *Memo2;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        TLabel *Label7;
        TButton *Button7;
        TMemo *Memo3;
        TButton *Button8;
        TButton *Button9;
        TButton *Button10;
        TButton *Button11;
        TButton *Button12;
        TButton *Button13;
        TButton *Button14;
        TButton *Button15;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall Button12Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button14Click(TObject *Sender);
        void __fastcall Button15Click(TObject *Sender);
private:	// User declarations
public:
           
        __fastcall TForm1(TComponent* Owner);
};
void disp(TMemo *Memo1, TMemo *Memo2);
void result(TMemo *Memo3);
void enter(TEdit *Edit1, TEdit *Edit2, TEdit *Edit3, TEdit *Edit4, TMemo *Memo1, TMemo *Memo2, TMemo *Memo3);
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
