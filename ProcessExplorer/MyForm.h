#pragma once
#include <Windows.h>

namespace PEf {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		static MyForm^ MyFormInst;
		MyForm(void)
		{
			InitializeComponent();
			MyFormInst = this;
		}


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  TitleLabel;
	public: System::Windows::Forms::Button^  ScanButton;
	private: System::Windows::Forms::Label^  InfoLabel;
	public: System::Windows::Forms::ListView^  ProcListView; // type ListView from the forms lib
	private: System::Windows::Forms::ColumnHeader^  Display_Name;
	private: System::Windows::Forms::ColumnHeader^  Display_pID;

	private: System::Windows::Forms::ColumnHeader^  Display_Mods;
	private: System::Windows::Forms::ColumnHeader^  Display_ModBsAdd;
	private: System::Windows::Forms::ColumnHeader^  Display_Threads;
	private: System::Windows::Forms::ColumnHeader^  Memory_Usage;

	private: System::Windows::Forms::ColumnHeader^  Display_Paths;
	private: System::Windows::Forms::Label^  detailsLabel;
	private: System::Windows::Forms::Label^  BottomLabel;











	private: System::ComponentModel::IContainer^  components;

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->TitleLabel = (gcnew System::Windows::Forms::Label());
			this->ScanButton = (gcnew System::Windows::Forms::Button());
			this->InfoLabel = (gcnew System::Windows::Forms::Label());
			this->ProcListView = (gcnew System::Windows::Forms::ListView());
			this->Display_Name = (gcnew System::Windows::Forms::ColumnHeader());
			this->Display_pID = (gcnew System::Windows::Forms::ColumnHeader());
			this->Display_Mods = (gcnew System::Windows::Forms::ColumnHeader());
			this->Display_ModBsAdd = (gcnew System::Windows::Forms::ColumnHeader());
			this->Display_Threads = (gcnew System::Windows::Forms::ColumnHeader());
			this->Memory_Usage = (gcnew System::Windows::Forms::ColumnHeader());
			this->Display_Paths = (gcnew System::Windows::Forms::ColumnHeader());
			this->detailsLabel = (gcnew System::Windows::Forms::Label());
			this->BottomLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// TitleLabel
			// 
			this->TitleLabel->AutoSize = true;
			this->TitleLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TitleLabel->Location = System::Drawing::Point(-9, 9);
			this->TitleLabel->MaximumSize = System::Drawing::Size(500, 30);
			this->TitleLabel->MinimumSize = System::Drawing::Size(500, 30);
			this->TitleLabel->Name = L"TitleLabel";
			this->TitleLabel->Size = System::Drawing::Size(500, 30);
			this->TitleLabel->TabIndex = 0;
			this->TitleLabel->Text = L"Windows Process Explorer (x86)";
			this->TitleLabel->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->TitleLabel->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// ScanButton
			// 
			this->ScanButton->Location = System::Drawing::Point(329, 334);
			this->ScanButton->Name = L"ScanButton";
			this->ScanButton->Size = System::Drawing::Size(125, 36);
			this->ScanButton->TabIndex = 1;
			this->ScanButton->Text = L"SCAN";
			this->ScanButton->UseVisualStyleBackColor = true;
			this->ScanButton->Click += gcnew System::EventHandler(this, &MyForm::ScanButton_Click);
			// 
			// InfoLabel
			// 
			this->InfoLabel->AutoSize = true;
			this->InfoLabel->Location = System::Drawing::Point(22, 39);
			this->InfoLabel->MaximumSize = System::Drawing::Size(450, 0);
			this->InfoLabel->Name = L"InfoLabel";
			this->InfoLabel->Size = System::Drawing::Size(432, 26);
			this->InfoLabel->TabIndex = 2;
			this->InfoLabel->Text = L"It is recommended to run this as admin.  This program will show information such "
				L"as current running processes, modules, paths, module base addresses, # of thread"
				L"s, priority, etc.";
			this->InfoLabel->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->InfoLabel->Click += gcnew System::EventHandler(this, &MyForm::label1_Click_2);
			// 
			// ProcListView
			// 
			this->ProcListView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {
				this->Display_Name,
					this->Display_pID, this->Display_Mods, this->Display_ModBsAdd, this->Display_Threads, this->Memory_Usage, this->Display_Paths
			});
			this->ProcListView->Location = System::Drawing::Point(12, 68);
			this->ProcListView->Name = L"ProcListView";
			this->ProcListView->ShowItemToolTips = true;
			this->ProcListView->Size = System::Drawing::Size(460, 260);
			this->ProcListView->TabIndex = 3;
			this->ProcListView->UseCompatibleStateImageBehavior = false;
			this->ProcListView->View = System::Windows::Forms::View::Details;
			this->ProcListView->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ProcListView_SelectedIndexChanged);
			// 
			// Display_Name
			// 
			this->Display_Name->Text = L"Name";
			// 
			// Display_pID
			// 
			this->Display_pID->Text = L"pID";
			this->Display_pID->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->Display_pID->Width = 40;
			// 
			// Display_Mods
			// 
			this->Display_Mods->Text = L"Modules";
			// 
			// Display_ModBsAdd
			// 
			this->Display_ModBsAdd->Text = L"Last Base Addr";
			this->Display_ModBsAdd->Width = 85;
			// 
			// Display_Threads
			// 
			this->Display_Threads->Text = L"Threads";
			this->Display_Threads->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->Display_Threads->Width = 55;
			// 
			// Memory_Usage
			// 
			this->Memory_Usage->Text = L"Memory Usage";
			this->Memory_Usage->Width = 55;
			// 
			// Display_Paths
			// 
			this->Display_Paths->Text = L"Path";
			this->Display_Paths->Width = 100;
			// 
			// detailsLabel
			// 
			this->detailsLabel->Location = System::Drawing::Point(12, 334);
			this->detailsLabel->Name = L"detailsLabel";
			this->detailsLabel->Size = System::Drawing::Size(311, 97);
			this->detailsLabel->TabIndex = 6;
			this->detailsLabel->Text = L"Information is not updated in real-time, it is only fetched once at initial scan.  If a name shows as Handle Failed, this may mean that the program identified a pID but the actual process is closed at handle fetch, or the process is owned by another user, etc.  To conserve space, only the last base address of a module is shown.  Addresses 0 indicate access was denied to the module or no module was found.";
			// 
			// BottomLabel
			// 
			this->BottomLabel->Location = System::Drawing::Point(12, 431);
			this->BottomLabel->Name = L"BottomLabel";
			this->BottomLabel->Size = System::Drawing::Size(311, 29);
			this->BottomLabel->TabIndex = 7;
			this->BottomLabel->Text = L"Note that this may take awhile to load every scanned process.";
			this->BottomLabel->Click += gcnew System::EventHandler(this, &MyForm::label1_Click_5);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(484, 561);
			this->Controls->Add(this->BottomLabel);
			this->Controls->Add(this->detailsLabel);
			this->Controls->Add(this->ProcListView);
			this->Controls->Add(this->InfoLabel);
			this->Controls->Add(this->ScanButton);
			this->Controls->Add(this->TitleLabel);
			this->MaximumSize = System::Drawing::Size(500, 600);
			this->MinimumSize = System::Drawing::Size(500, 600);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	public: System::Void ScanButton_Click(System::Object^  sender, System::EventArgs^  e);  //  Define here, have implementation in cpp

	private: System::Void label1_Click_1(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label1_Click_2(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
		//  Item 1, look here https://www.youtube.com/watch?v=lXgWVZSZmVA
		/*ListViewItem^ listViewItem = gcnew Windows::Forms::ListViewItem("test");
		listViewItem->SubItems->Add("hello");
		listViewItem->SubItems->Add("hello2");
		ListViewItem^ testNew = gcnew Windows::Forms::ListViewItem("test2");
		testNew->SubItems->Add("hello3");
		this->ProcListView->Items->Add(listViewItem);
		this->ProcListView->Items->Add(testNew); */

	}
	private: System::Void label1_Click_3(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void ProcListView_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label1_Click_4(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void label1_Click_5(System::Object^  sender, System::EventArgs^  e) {
	}
};


}
