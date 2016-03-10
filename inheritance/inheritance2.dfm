object Form1: TForm1
  Left = 189
  Top = 163
  Width = 987
  Height = 583
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
  object Label17: TLabel
    Left = 248
    Top = 8
    Width = 28
    Height = 13
    Caption = 'Count'
  end
  object GroupBox7: TGroupBox
    Left = 584
    Top = 8
    Width = 369
    Height = 353
    Caption = 'List of workers'
    TabOrder = 0
    object ComboBox1: TComboBox
      Left = 8
      Top = 24
      Width = 361
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      Text = 'ALL'
      OnChange = ComboBox1Change
    end
    object ListBox1: TListBox
      Left = 8
      Top = 48
      Width = 353
      Height = 217
      ItemHeight = 13
      TabOrder = 1
    end
    object Button7: TButton
      Left = 16
      Top = 280
      Width = 81
      Height = 25
      Caption = 'Delete'
      TabOrder = 2
      OnClick = Button7Click
    end
    object Button8: TButton
      Left = 16
      Top = 304
      Width = 81
      Height = 25
      Caption = 'Edit'
      TabOrder = 3
      OnClick = Button8Click
    end
    object GroupBox8: TGroupBox
      Left = 120
      Top = 184
      Width = 225
      Height = 145
      Caption = 'EDITING'
      TabOrder = 4
      Visible = False
      object Label1: TLabel
        Left = 8
        Top = 16
        Width = 28
        Height = 13
        Caption = 'Name'
      end
      object Label21: TLabel
        Left = 8
        Top = 40
        Width = 11
        Height = 13
        Caption = 'ID'
      end
      object Label22: TLabel
        Left = 8
        Top = 64
        Width = 20
        Height = 13
        Caption = 'Title'
      end
      object Label23: TLabel
        Left = 8
        Top = 88
        Width = 19
        Height = 13
        Caption = 'Age'
      end
      object Label24: TLabel
        Left = 8
        Top = 112
        Width = 57
        Height = 13
        Caption = 'Publications'
      end
      object Button9: TButton
        Left = 168
        Top = 56
        Width = 41
        Height = 41
        Caption = 'OK'
        TabOrder = 0
        OnClick = Button9Click
      end
    end
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 201
    Height = 137
    Caption = 'Student'
    TabOrder = 1
    object Label2: TLabel
      Left = 8
      Top = 16
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object Label3: TLabel
      Left = 8
      Top = 48
      Width = 11
      Height = 13
      Caption = 'ID'
    end
    object Label4: TLabel
      Left = 8
      Top = 80
      Width = 57
      Height = 13
      Caption = 'Publications'
    end
    object Edit1: TEdit
      Left = 88
      Top = 16
      Width = 105
      Height = 21
      TabOrder = 0
      Text = 'Student2'
    end
    object Edit2: TEdit
      Left = 88
      Top = 48
      Width = 105
      Height = 21
      TabOrder = 1
      Text = '12'
    end
    object Edit3: TEdit
      Left = 88
      Top = 80
      Width = 105
      Height = 21
      TabOrder = 2
      Text = '3'
    end
    object Button1: TButton
      Left = 56
      Top = 104
      Width = 105
      Height = 25
      Caption = 'Add'
      TabOrder = 3
      OnClick = Button1Click
    end
  end
  object GroupBox2: TGroupBox
    Left = 240
    Top = 64
    Width = 201
    Height = 113
    Caption = 'Laborer'
    TabOrder = 2
    object Label5: TLabel
      Left = 8
      Top = 16
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object Label6: TLabel
      Left = 8
      Top = 48
      Width = 11
      Height = 13
      Caption = 'ID'
    end
    object Edit4: TEdit
      Left = 88
      Top = 16
      Width = 105
      Height = 21
      TabOrder = 0
      Text = 'Laborer2'
    end
    object Edit5: TEdit
      Left = 88
      Top = 48
      Width = 105
      Height = 21
      TabOrder = 1
      Text = '11142'
    end
    object Button4: TButton
      Left = 64
      Top = 72
      Width = 89
      Height = 25
      Caption = 'Add'
      TabOrder = 2
      OnClick = Button4Click
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 168
    Width = 201
    Height = 137
    Caption = 'Researcher'
    TabOrder = 3
    object Label7: TLabel
      Left = 8
      Top = 16
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object Label8: TLabel
      Left = 8
      Top = 48
      Width = 11
      Height = 13
      Caption = 'ID'
    end
    object Label9: TLabel
      Left = 8
      Top = 80
      Width = 57
      Height = 13
      Caption = 'Publications'
    end
    object Edit6: TEdit
      Left = 88
      Top = 16
      Width = 105
      Height = 21
      TabOrder = 0
      Text = 'Researcher2'
    end
    object Edit7: TEdit
      Left = 88
      Top = 48
      Width = 105
      Height = 21
      TabOrder = 1
      Text = '122'
    end
    object Edit8: TEdit
      Left = 88
      Top = 80
      Width = 105
      Height = 21
      TabOrder = 2
      Text = '10'
    end
    object Button2: TButton
      Left = 56
      Top = 104
      Width = 105
      Height = 25
      Caption = 'Add'
      TabOrder = 3
      OnClick = Button2Click
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 328
    Width = 201
    Height = 137
    Caption = 'Professor'
    TabOrder = 4
    object Label10: TLabel
      Left = 8
      Top = 16
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object Label11: TLabel
      Left = 8
      Top = 48
      Width = 11
      Height = 13
      Caption = 'ID'
    end
    object Label12: TLabel
      Left = 8
      Top = 80
      Width = 57
      Height = 13
      Caption = 'Publications'
    end
    object Edit9: TEdit
      Left = 88
      Top = 16
      Width = 105
      Height = 21
      TabOrder = 0
      Text = 'Professor2'
    end
    object Edit10: TEdit
      Left = 88
      Top = 48
      Width = 105
      Height = 21
      TabOrder = 1
      Text = '1132'
    end
    object Edit11: TEdit
      Left = 88
      Top = 80
      Width = 105
      Height = 21
      TabOrder = 2
      Text = '42'
    end
    object Button3: TButton
      Left = 56
      Top = 104
      Width = 105
      Height = 25
      Caption = 'Add'
      TabOrder = 3
      OnClick = Button3Click
    end
  end
  object GroupBox5: TGroupBox
    Left = 240
    Top = 200
    Width = 201
    Height = 177
    Caption = 'Manager'
    TabOrder = 5
    object Label13: TLabel
      Left = 8
      Top = 16
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object Label14: TLabel
      Left = 8
      Top = 48
      Width = 11
      Height = 13
      Caption = 'ID'
    end
    object Label15: TLabel
      Left = 8
      Top = 80
      Width = 20
      Height = 13
      Caption = 'Title'
    end
    object Label16: TLabel
      Left = 8
      Top = 112
      Width = 19
      Height = 13
      Caption = 'Age'
    end
    object Edit12: TEdit
      Left = 88
      Top = 16
      Width = 105
      Height = 21
      TabOrder = 0
      Text = 'Manager2'
    end
    object Edit13: TEdit
      Left = 88
      Top = 48
      Width = 105
      Height = 21
      TabOrder = 1
      Text = '11152'
    end
    object Edit14: TEdit
      Left = 88
      Top = 80
      Width = 105
      Height = 21
      TabOrder = 2
      Text = 'Group1 Manager'
    end
    object Edit15: TEdit
      Left = 88
      Top = 112
      Width = 105
      Height = 21
      TabOrder = 3
      Text = '25'
    end
    object Button5: TButton
      Left = 64
      Top = 136
      Width = 97
      Height = 25
      Caption = 'Add'
      TabOrder = 4
      OnClick = Button5Click
    end
  end
  object Edit16: TEdit
    Left = 248
    Top = 24
    Width = 49
    Height = 21
    TabOrder = 6
    Text = 'Edit16'
  end
  object GroupBox6: TGroupBox
    Left = 632
    Top = 368
    Width = 321
    Height = 169
    Caption = 'Searching'
    TabOrder = 7
    object Label18: TLabel
      Left = 8
      Top = 48
      Width = 26
      Height = 13
      Caption = 'Fraze'
    end
    object Label19: TLabel
      Left = 144
      Top = 72
      Width = 30
      Height = 13
      Caption = 'Result'
    end
    object Label20: TLabel
      Left = 8
      Top = 24
      Width = 29
      Height = 13
      Caption = 'Group'
    end
    object Edit17: TEdit
      Left = 40
      Top = 48
      Width = 153
      Height = 21
      TabOrder = 0
    end
    object ComboBox2: TComboBox
      Left = 40
      Top = 24
      Width = 153
      Height = 21
      ItemHeight = 13
      TabOrder = 1
    end
    object ListBox2: TListBox
      Left = 24
      Top = 88
      Width = 273
      Height = 57
      ItemHeight = 13
      TabOrder = 2
    end
    object Button6: TButton
      Left = 128
      Top = 144
      Width = 65
      Height = 17
      Caption = 'Search'
      TabOrder = 3
      OnClick = Button6Click
    end
  end
  object Edit22: TEdit
    Left = 784
    Top = 300
    Width = 89
    Height = 21
    TabOrder = 8
    Visible = False
  end
  object Edit21: TEdit
    Left = 784
    Top = 276
    Width = 89
    Height = 21
    TabOrder = 9
    Visible = False
  end
  object Edit20: TEdit
    Left = 784
    Top = 252
    Width = 89
    Height = 21
    TabOrder = 10
    Visible = False
  end
  object Edit19: TEdit
    Left = 784
    Top = 232
    Width = 89
    Height = 21
    TabOrder = 11
    Visible = False
  end
  object Edit18: TEdit
    Left = 784
    Top = 208
    Width = 89
    Height = 21
    TabOrder = 12
    Visible = False
  end
end
