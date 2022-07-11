#include <iostream> //standart input output
#include <stdlib.h> //header untuk operasi pembanding atau operasi konversi
#include <conio.h> //Menampilkan hasil antarmuka kepada pengguna
#include <windows.h> //header yang berisi deklarasi untuk semua fungsi di windows API
#include <ctime> // mengonversi waktu yang diberikan ke waktu lokal kalender dan kemudian ke representasi karakter.
#define max 5
using namespace std;

int tail = -1; //tail untuk queue

struct Menu
{
	string nama;
	int stok, harga, sold, uang;
}menu[12];

struct Pesanan
{
	int menu;					
	int jumlah;
};
struct Cashier
{
	string clock;					// variabel untuk menyimpan waktu saat pesan
	int total;						// variabel untuk menampung total penjualan
	string cust;					// Variabel untuk menampung nama orang
    int price{}; 					// Variabel untuk menyimpan total harga per 1 antrian
    string note;					// Variabel untuk catatan dari pelanggan

	Pesanan pesan[12];
	int count{};						//variabel untuk menampung jumlah pesanan

	void hitungHarga()
	{
		for(int i = 0; i<count; i++)
		{
			price+=(menu[pesan[i].menu].harga)*pesan[i].jumlah; 
			menu[pesan[i].menu].stok -= pesan[i].jumlah; // untuk mengurangi data stock per item
			menu[pesan[i].menu].sold += pesan[i].jumlah; // untuk menambah data sold per item
		}
	}
};

void gotoxy(short x, short y)//Fungsi Untuk mengatur/menentukan letak output suatu program
{ 
	//menunjukkan posisi kursor baru dalam karakter pada layar output.
	COORD pos = {x, y};
	//mengeset posisi
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
char inputChar(char a, char b)
{
	char input;
	while(true)
	{
		cin.clear(); //membersihkan flag kesalahan pada cin
		cin >> input;
		if(tolower(input) == a || tolower(input) == b) return input; //convert character to lowercase
		cout << "input salah ulangi: ";
	}
	
}

bool kosong() //fungsi untuk cek data kosong/tidak
{
    if(tail == -1){
		return true;
	}else{
		return false;
	}
}
bool penuh() //fungsi untuk cek data penuh/tidak
{
   if(tail == max){
		return true;
	}else{
		return false;
	}
}

// Fungsi-fungsi input Pesanan
void tampilkanMenu()
{
	cout << "|=============================================================|\n";
	cout << "|\t\t\t  MENU PESANAN\t\t\t      |\n";
	cout << "|=============================================================|\n";
	cout << "|   Menu Makanan   |   Harga   |   Menu Minuman   |   Harga   |\n";
	cout << "|=============================================================|\n";
	for(int i = 0 ; i < 6; i++)
	{
		gotoxy(0,i+5); cout <<"|"<< i+1 <<".";
		gotoxy(4,i+5); cout <<menu[i].nama;
		gotoxy(19,i+5); cout << "| Rp." <<menu[i].harga;
		gotoxy(31,i+5); cout <<"|"<< i+7 <<".";
		gotoxy(35,i+5); cout <<menu[i+6].nama;
		gotoxy(50,i+5); cout << "| Rp." <<menu[i+6].harga;
		gotoxy(62,i+5); cout <<"|";
	}
	cout << "\n|=============================================================|\n";
}
void waktu(Cashier *data) // fungsi set time pemesanan
{
	// tanggal atau waktu saat ini berdasarkan sistem saat ini
	time_t a=time(0);

	// konversi sekarang ke bentuk string dan disimpen ke dalam data.clock
	data[tail].clock = ctime(&a);
}
void input(Cashier *data) // fungsi untuk memasukan data customer
{

	system("cls");
	int pesanan,jumlah = 0,total = 0, price;
	char kondisi;
	int i = 0;
	while(true)
	{
		system("cls");
		tampilkanMenu();
		cout << " Menu yang ingin dipesan: ";
		cin >> pesanan; 
		ulangimemesan:
		cout << " Jumlah: ";
		cin >> jumlah;

		data[tail].pesan[i].menu = pesanan - 1; // memasukan menu yang dipesan
		data[tail].pesan[i].jumlah = jumlah;  // memasukan jumlah menu yang dipesan
		
		// validasi stok
		if(jumlah > menu[pesanan-1].stok)
		{  
			
			cout << " Stok persediaan tersisa " << menu[pesanan-1].stok <<"\n";
			cout << " Apakah anda masih ingin memesan " << menu[pesanan-1].nama << "? (y/n) ";
			cin >> kondisi;
			if(tolower(kondisi) == 'y')goto ulangimemesan;
			else if(tolower(kondisi) == 'n')
			{
				tail--;
				return;
			}
		}
		else if(jumlah <= menu[pesanan-1].stok)
		{
			data[tail].count++; // untuk menghitung berapa banyak item yang sudah dimasukan
			cout << " Tambah Pesanan? Y/N: ";
			if(inputChar('y', 'n') == 'n') break; //mengakhiri loop
			i++;
		}
	}
	data[tail].hitungHarga();

	cout << "|=============================================================|\n";
	cout << " Input nama pemesan: ";
	cin.ignore();
	getline (cin, data[tail].cust);	 // memasukan nama customer
	cout << " Catatan Customer: \n";
	cout << " ";
	getline (cin, data[tail].note); // memasukan catatan atau pesan dari customer
	cout << "|=============================================================|\n";
	cout << " Terimakasih" << endl;
	waktu(data); // fungsi untuk menyimpan waktu saat proses pesan
	cout << endl; 
}
void enqueue(Cashier *data) //fungsi untuk menambah data antrian
{
	if (!penuh())
    {
		tail++; 
        input(data);
    }
    else
    {
        cout << "Antrian sudah penuh!!";
		cin.get();
    }
	getchar();
	
}

// Fungsi-fungsi bayar Pesanan
void pay(Cashier *data) // fungsi bayar sebelum hapus antrian
{
	int i=0,payment,change,kondisi=0;
	do
	{
		system("cls");
		cout << "|=============================================================|\n";
		cout << "| Antrian pesan ke-" << i + 1<< "\t\t\t\t\t      |\n";
		cout << "|=============================================================|\n";
		cout << "| Tanggal Transaksi: " << data[i].clock;
		gotoxy(62,3); cout <<"|";
		cout << "\n| Pesanan atas nama: " << data[i].cust << endl; 
		gotoxy(62,4); cout <<"|";
		cout << "\n|-------------------------------------------------------------|\n";
		for(int j=0; j<data[i].count; j++)
		{
			gotoxy(0,j+6); cout <<"| " << menu[data[i].pesan[j].menu].nama ;
			gotoxy(18,j+6); cout <<"| " << data[i].pesan[j].jumlah;
			gotoxy(28,j+6); cout <<"| Rp." << menu[data[i].pesan[j].menu].harga;
			gotoxy(45,j+6); cout << "| Rp." << data[i].pesan[j].jumlah * menu[data[i].pesan[j].menu].harga << "\n"; 
			gotoxy(62,j+6); cout <<"|";		
		}
		cout << "\n|-------------------------------------------------------------|\n";
		cout << " Total Harga: Rp." << data[i].price << endl;
		cout << " Dibayar: Rp."; cin >> payment;
		if (payment < data[i].price )
		{
			cout << "Jumlah yang dimasukan kurang!!!\n";
			getch(); // Membaca input dari komputer
			continue; //untuk melewati iterasi loop saat ini dan kontrol program
		}
		else if (payment == data[i].price)
		{
			cout << " Uang pas" << endl;
			kondisi = 1;
			getch();
		} 
		else if (payment > data[i].price )
		{		
			change = payment - data[i].price;
			cout << " Kembali: Rp." << change;
			kondisi = 1;
			getch();
		}
	} while (kondisi == 0);
	cout << "\n|=============================================================|\n";
	cout << "\n Pembayaran berhasil";
	
}
void dequeue(Cashier *data) //fungsi untuk mengurangi data antrian
{
    if(!kosong()){
		pay(data);
		for(int i = 0; i <= tail; i++)
		{
			data[i] = data[i + 1];
		}
		tail--;
		
	}else{
		cout << "Antrian kosong";
		getch(); // Membaca input dari komputer
		return;
	}
}

// Fungsi tampilkan urutan pesanan Pesanan
void tampilData(Cashier *data) //fungsi untuk menampilkan data
{
	system("cls"); 
    if(!kosong())
	{
		for(int i = 0; i <= tail; i++)
		{	
			cout << "|=============================================================|\n";
			cout << "| Antrian pesan ke-" << i + 1<< "\t\t\t\t\t      |\n";
			cout << "|=============================================================|\n";
			cout << "| Tanggal Transaksi: " << data[i].clock;
			cout << "| Pesanan atas nama: " << data[i].cust << "\n"; 
			cout << "|-------------------------------------------------------------|\n";
			for(int j=0; j<data[i].count; j++)
			{
				cout << "\t" << data[i].pesan[j].jumlah << "\t|\t" << menu[data[i].pesan[j].menu].nama  << "\n";
			}
			cout << "|-------------------------------------------------------------|\n";
			cout << " Harga total: Rp." << data[i].price << endl;
			cout << " Catatan Customer: \n" << " " << data[i].note << endl;
			cout << "|=============================================================|\n\n";
		}
		
	}
	else{
		cout << "Antrian masih kosong !!!";
	}
	cout << endl;
	getch(); // Membaca input dari komputer
}

// Fungsi-fungsi menampilkan stok dan tambah stok
void sortiename(string *name, int *stock, int *sell, int *price) //fungsi untuk mengurutkan data berdasarkan nama menggunakan bubble sort secara ascending 
{
    string tmp;
	int tmp1, tmp2, tmp3;
    int i, j;
    for(i=0; i<6; i++)					// sorting Makanan berdasarkan nama
    {
        tmp = name[i];
		tmp1 = stock[i];
		tmp2 = sell[i];
		tmp3 = price[i];
        j = i-1;
        while(j>=0 && name[j] > tmp)
        {
            name[j+1] = name[j];
			stock[j+1] = stock[j];
			sell[j+1] = sell[j];
			price[j+1] = price[j];
            j--;
        }
        name[j+1] = tmp;
		stock[j+1] = tmp1;
		sell[j+1] = tmp2;
		price[j+1] = tmp3;
    }
	for(i=6; i<12; i++)					// sorting Minuman berdasarkan nama
    {
        tmp = name[i];
		tmp1 = stock[i];
		tmp2 = sell[i];
		tmp3 = price[i];
        j = i-1;
        while(j>=6 && name[j] > tmp)
        {
            name[j+1] = name[j];
			stock[j+1] = stock[j];
			sell[j+1] = sell[j];
			price[j+1] = price[j];
            j--;
        }
        name[j+1] = tmp;
		stock[j+1] = tmp1;
		sell[j+1] = tmp2;
		price[j+1] = tmp3;
    }
}
void sortiestock(string *name, int *stock, int *sell, int *price) //fungsi untuk mengurutkan data berdasarkan Stok menggunakan bubble sort secara ascending 
{
    string tmp;
	int tmp1, tmp2, tmp3;
    int i, j;
    for(i=0; i<6; i++)
    {
        tmp = name[i];
		tmp1 = stock[i];
		tmp2 = sell[i];
		tmp3 = price[i];
        j = i-1;
        while(j>=0 && stock[j] > tmp1)		// sorting Makanan berdasarkan stok
        {
            name[j+1] = name[j];
			stock[j+1] = stock[j];
			sell[j+1] = sell[j];
			price[j+1] = price[j];
            j--;
        }
        name[j+1] = tmp;
		stock[j+1] = tmp1;
		sell[j+1] = tmp2;
		price[j+1] = tmp3;
    }
	for(i=6; i<12; i++)						// sorting Minuman berdasarkan stok
    {
        tmp = name[i];
		tmp1 = stock[i];
		tmp2 = sell[i];
		tmp3 = price[i];
        j = i-1;
        while(j>=6 && stock[j] > tmp1)
        {
            name[j+1] = name[j];
			stock[j+1] = stock[j];
			sell[j+1] = sell[j];
			price[j+1] = price[j];
            j--;
        }
        name[j+1] = tmp;
		stock[j+1] = tmp1;
		sell[j+1] = tmp2;
		price[j+1] = tmp3;
    }
}
void searching(Cashier *data) // fungsi untuk menampilkan stok dan menambahkan stok
{
	// alokasi memori dinamis
	int *stok = new int[12]; 
	int *sold = new int[12];
	int *uang = new int[12];
	string *nama = new string[12];

	int phill;
	char kondisi;
	data->total = 0;
	back:
    for(int i=0; i<12; i++)
	{
		data->total += (menu[i].harga * menu[i].sold);
		menu[i].uang = (menu[i].harga * menu[i].sold);
		nama[i] = menu[i].nama;
		stok[i] = menu[i].stok;
		sold[i] = menu[i].sold;
		uang[i] = menu[i].uang;
    }

	do
	{
		system("cls"); 
		cout << "|==========================================================|\n";
		cout << "|\t\t\tMenu Sorting\t\t\t   |\n";		
		cout << "|==========================================================|\n";
		cout << "| [1] Berdasarkan nama                                     |\n";
		cout << "| [2] Berdasarkan stok                                     |\n";
		cout << "|==========================================================|\n";
		cout << " Masukan: ";
		cin >> phill;
		if(phill == 1)
		{
			// sorting makan dan minuman berdasarkan nama menggunakan bubble sort secara ascending 
			sortiename(nama, stok, sold, uang);
			break; //mengakhiri loop
		}
		else if(phill == 2)
		{
			// sorting makan dan minuman berdasarkan jumlah stok menggunakan bubble sort secara ascending 
			sortiestock(nama, stok, sold, uang);
			break; //mengakhiri loop
		}
		cout << " Input yang anda masukan salah!!!\n";
		getch(); // Membaca input dari komputer
	} while (true);
	system("cls");
    cout << "|==========================================================|\n";
	cout << "|\t\t    Daftar Hasil Penjualan\t\t   |\n";
	cout << "|==========================================================|\n";
	cout << "|No.|    Nama     |   Stok   |   Jumlah    |     Hasil     |\n";
	cout << "|   |   Makanan   |  Makanan |  Penjualan  |   Penjualan   |\n";
	cout << "|==========================================================|\n";
	for(int i=0; i<6; i++){
		gotoxy(0,i+6);cout <<"|"<< i+1 <<".";
		gotoxy(4,i+6);cout <<"|"<< nama[i];
		gotoxy(18,i+6); cout <<"|    "<<stok[i];
		gotoxy(29,i+6); cout <<"|      "<<sold[i];
		gotoxy(43,i+6); cout <<"| Rp." <<uang[i];
		gotoxy(59,i+6); cout <<"|";
	}
	cout << "\n|==========================================================|\n";
	cout << "|No.|    Nama     |   Stok   |   Jumlah    |     Hasil     |\n";
	cout << "|   |   Minuman   |  Minuman |  Penjualan  |   Penjualan   |\n";
	cout << "|==========================================================|\n";
	for(int i=6; i<12; i++){
		gotoxy(0,i+10);cout <<"|"<< i+1 <<".";
		gotoxy(4,i+10);cout <<"|"<< nama[i];
		gotoxy(18,i+10); cout <<"|    "<<stok[i];
		gotoxy(29,i+10); cout <<"|      "<<sold[i];
		gotoxy(43,i+10); cout <<"| Rp." <<uang[i];
		gotoxy(59,i+10); cout <<"|";
	}
	cout << "\n|==========================================================|\n";
    cout << "| Total penjualan pada hari ini adalah Rp." << data->total;
	gotoxy(59,23); cout <<"|";
	cout << "\n|==========================================================|\n";
    cout << "Tekan 'y' untuk mengisi stok\ntekan 'n' untuk ke menu utama\nMasukan pilihan: ";
	cin >> kondisi;
	if(tolower(kondisi) == 'y') //convert character to lowercase
	{
		string cari;
		int replace;
		// Searching menggunakan Sequential Search
		cout<<"\nMasukan nama item yang ingin di tambahkan: ";
		cin.ignore();
		getline(cin, cari);
		int ketemu=0;
		for(int i=0; i<=12; i++)
		{
			if(menu[i].nama == cari) // proses menambah jumlah stok
			{
				cout << "Inputkan jumlah stok: ";
				cin >> replace;
				menu[i].stok += replace;
				ketemu = 1;
				goto back;
			}
		}
		if(ketemu == 0)
		{
		cout<<"Data tidak ditemukan";
		}
	}
	if(tolower(kondisi) == 'n') //convert character to lowercase
	{
		return;
	}
	getch(); // Membaca input dari komputer
	// delokasi memori dinamis
	delete[] stok;
	delete[] sold;
	delete[] uang;
	delete[] nama;
}
void inputStok() // fungsi untuk memasukan data data ke dalam struct menu
{
	string nama[12] = {"Bakso", "Bubur Ayam", "Nasi Goreng", "Nasi Padang", "Pecel", "Soto",
						"Airputih", "Cola", "Jeruk", "Jus", "Lemon", "Teh"};
	int jumlah[12] = {23, 32, 42, 20, 45, 34,
					   31, 18, 21, 33, 23, 43};
	int harga[12] = {10000,8000,15000,12000,9000,8000,2000,6000,3000,10000,7000,3000};
	for(int i = 0; i<12; i++)
	{
		menu[i] = {nama[i], jumlah[i], harga[i]};
	}
}

int main()
{
	system("color F0");	 // Merubah tampilan background menjadi bright white dan foreground menjadi hitam
	Cashier *data = new Cashier[max]{}; 		// alokasi memori dinamis
	inputStok();
	time_t a=time(0);
    int pil;
    do 
    {
		system("cls");
		cout << "|==========================================================|\n";
        cout << "|\t\t\t Menu Utama\t\t\t   |\n";
        cout << "|==========================================================|\n";
        cout << "| [1] Input Pesanan                                        |\n";
        cout << "| [2] Pembayaran Pesanan                                   |\n";
        cout << "| [3] List Antrian                                         |\n";
		cout << "| [4] Menu Stok                                            |\n";
        cout << "| [5] Exit Program                                         |\n";
        cout << "|==========================================================|\n";
        cout << " Masukan pilihan : ";cin>>pil;
        switch (pil)
        {
            case 1:
                enqueue(data);
                break; //mengakhiri loop
            case 2:
                dequeue(data);
                break; //mengakhiri loop
            case 3:
                tampilData(data);
                break; //mengakhiri loop
			case 4:
				searching(data);
				break; //mengakhiri loop
			case 5:
				cout << "Terimakasih" << endl;
        }
    }
    while(pil<5);
	delete[] data; 						// delokasi memori dinamis
	system("cls");
	cout << "Terimakasih" << endl;
	cin.get();
    return 0;
}