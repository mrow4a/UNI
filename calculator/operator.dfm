object Form1: TForm1
  Left = 245
  Top = 206
  Width = 979
  Height = 563
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 216
    Top = 42
    Width = 7
    Height = 16
    Caption = '+'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 272
    Top = 43
    Width = 2
    Height = 13
    Alignment = taCenter
    Caption = 'i'
  end
  object Label3: TLabel
    Left = 274
    Top = 67
    Width = 2
    Height = 13
    Alignment = taCenter
    Caption = 'i'
  end
  object Label4: TLabel
    Left = 217
    Top = 66
    Width = 7
    Height = 16
    Caption = '+'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 24
    Top = 40
    Width = 99
    Height = 13
    Caption = 'First complex number'
  end
  object Label6: TLabel
    Left = 24
    Top = 64
    Width = 117
    Height = 13
    Caption = 'Second complex number'
  end
  object Label7: TLabel
    Left = 360
    Top = 384
    Width = 43
    Height = 13
    Caption = 'RESULT'
  end
  object Edit1: TEdit
    Left = 160
    Top = 40
    Width = 57
    Height = 21
    TabOrder = 0
    Text = '4'
  end
  object Edit2: TEdit
    Left = 224
    Top = 40
    Width = 49
    Height = 21
    TabOrder = 1
    Text = '3'
  end
  object Edit3: TEdit
    Left = 160
    Top = 64
    Width = 57
    Height = 21
    TabOrder = 2
    Text = '6'
  end
  object Edit4: TEdit
    Left = 224
    Top = 64
    Width = 49
    Height = 21
    TabOrder = 3
    Text = '-2'
  end
  object Button1: TButton
    Left = 168
    Top = 96
    Width = 97
    Height = 25
    Caption = 'Enter values'
    TabOrder = 4
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 24
    Top = 224
    Width = 281
    Height = 33
    Alignment = taCenter
    Lines.Strings = (
      'Memo1')
    ReadOnly = True
    TabOrder = 5
  end
  object Memo2: TMemo
    Left = 456
    Top = 224
    Width = 273
    Height = 33
    Alignment = taCenter
    Lines.Strings = (
      'Memo2')
    ReadOnly = True
    TabOrder = 6
  end
  object Button2: TButton
    Left = 320
    Top = 144
    Width = 121
    Height = 25
    Caption = 'Increment real part'
    TabOrder = 7
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 320
    Top = 176
    Width = 121
    Height = 25
    Caption = 'Decrement real part'
    TabOrder = 8
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 320
    Top = 208
    Width = 25
    Height = 25
    Caption = '+'
    TabOrder = 9
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 352
    Top = 208
    Width = 25
    Height = 25
    Caption = '-'
    TabOrder = 10
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 384
    Top = 208
    Width = 25
    Height = 25
    Caption = '*'
    TabOrder = 11
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 416
    Top = 208
    Width = 25
    Height = 25
    Caption = '/'
    TabOrder = 12
    OnClick = Button7Click
  end
  object Memo3: TMemo
    Left = 216
    Top = 408
    Width = 321
    Height = 33
    Alignment = taCenter
    Lines.Strings = (
      'Memo3')
    TabOrder = 13
  end
  object Button8: TButton
    Left = 320
    Top = 240
    Width = 25
    Height = 25
    Caption = '+='
    TabOrder = 14
    OnClick = Button8Click
  end
  object Button9: TButton
    Left = 352
    Top = 240
    Width = 25
    Height = 25
    Caption = '-='
    TabOrder = 15
    OnClick = Button9Click
  end
  object Button10: TButton
    Left = 384
    Top = 240
    Width = 25
    Height = 25
    Caption = '*='
    TabOrder = 16
    OnClick = Button10Click
  end
  object Button11: TButton
    Left = 416
    Top = 240
    Width = 25
    Height = 25
    Caption = '/='
    TabOrder = 17
    OnClick = Button11Click
  end
  object Button12: TButton
    Left = 320
    Top = 272
    Width = 25
    Height = 25
    Caption = '>'
    TabOrder = 18
    OnClick = Button12Click
  end
  object Button13: TButton
    Left = 352
    Top = 272
    Width = 25
    Height = 25
    Caption = '<'
    TabOrder = 19
    OnClick = Button13Click
  end
  object Button14: TButton
    Left = 384
    Top = 272
    Width = 25
    Height = 25
    Caption = '>='
    TabOrder = 20
    OnClick = Button14Click
  end
  object Button15: TButton
    Left = 416
    Top = 272
    Width = 25
    Height = 25
    Caption = '<='
    TabOrder = 21
    OnClick = Button15Click
  end
end
