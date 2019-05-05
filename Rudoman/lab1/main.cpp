#include <iostream>

using namespace std;

int q = 0;

int space(int** Sqr, int N, int* x, int *y) {   //функция поиска свободного места
    for(*x = 0; *x < N; (*x)++) {
        for(*y = 0; *y < N; (*y)++) {
            if(Sqr[*x][*y] == 0)
                return 0;
        }
    }
    return 1;
}

int length(int** Sqr, int N, int x, int y) {    //функция поиска длины стороны вставляемого квадрата
    int L = 0;
    for(; ((L + x) < N) && ((L + y) < N); L++) {
        if(Sqr[x + L][y] != 0 || Sqr[x][y + L] != 0) {
            break;
        }
    }
    return L;
}

void full(int** Sqr, int L, int x, int y, int color) { //функция вставки квадрата
    for (int a = 0; a < L; a++)
        for (int b = 0; b < L; b++)
            Sqr[x+a][y+b] = color;
}

int find_full(int** Sqr, int N, int* x, int *y){ //функция поиска вставленного квадрата
    for(*y = 0; *y < N; (*y)++)  {
        for(*x = 0; *x < N; (*x)++){
            if(Sqr[*x][*y] != 0)
                return 0;
        }
    }
    return 1;
}

void rec(int** Sqr, int** minSqr, int N, int color, int* min) { //рекурсивная функция
    q++;
    if(color > *min)            //если количество цветов в текущей раскраске уже превышает минимальную,
        return;                 //то возвращаемся назад
    int x,y;
    int Ney1 = space(Sqr, N, &x, &y);

    if( Ney1 == 0) {
        int l = length(Sqr, N, x, y);
        if (l == N)
            l--;
        for(; l > 0; l--) {      //цикл, чтобы при разматывании поставить квадрат меньше и найти другое решение
            full(Sqr, l, x, y, l);
            rec(Sqr, minSqr, N, color+1, min);
            full(Sqr, l, x, y, 0);
        }
    }

    else {
        if(color < *min) {  //запоминание минимальной раскраски
            for(x = 0; x < N; x++) {        
                for(y =0; y < N; y++) {
                    
                    minSqr[x][y] = Sqr[x][y];
                }
            }
            *min = color;
        }
    }

}

int main() {
    int N;
    cin >> N;
    int answer;
    int zoom = 1; //переменная для масштабирования
        int** Square;
        int** miniSquare;

    if(N % 2 != 0){ //если длина квадрата нечетная    
        if(N%3 == 0 && N!= 3)
            zoom = 3;
        if(N%5 == 0 && N!= 5)
            zoom = 5;
        
        int new_N = N/zoom; //длина стороны квадрата с учетом масштаба
    	Square = new int*[N/zoom];               //создание массивов для хранения текущего и минимального решения
   	 	miniSquare = new int*[N/zoom];           
    	
        for(int i = 0; i < N/zoom; i++) {        
        	Square[i] = new int[N/zoom];
        	miniSquare[i] = new int[N/zoom];
    	}

    	for(int x = 0; x < N/zoom; x++) {        //заполнение массивов нулями
        	for(int y = 0; y < N/zoom; y++) {    
            miniSquare[x][y] = 0;
            Square[x][y] = 0;
        	}
    	}

        full(Square,new_N/2+1,0,0,new_N/2+1);       //заполнение первыми тремя квадратами для минмального решения
        full(Square,new_N/2,new_N/2+1,0,new_N/2);
        full(Square,new_N/2,0,new_N/2+1,new_N/2);

        answer = 2*N+1;                 // максимальное количество квадратов + 1
        
        rec(Square, miniSquare, N/zoom, 3, &answer);
        int x = 0, y =0;
        cout << answer << endl;

     while(find_full(miniSquare,N/zoom,&x,&y) == 0){  //вывод ответа в формате координаты и длина 
        int tmp = miniSquare[x][y];
        cout << x*zoom +1<< ' ' << y*zoom +1<< ' ' << tmp*zoom << endl;
        full(miniSquare, tmp,x, y, 0);

     }

     for(int i = 0; i < N/zoom; i++) {        
            delete [] Square[i];
            delete [] miniSquare[i];
     }
        delete [] Square;
        delete [] miniSquare;

    }
    
    else{                                       //если длина стороны четная
       cout << 4 <<endl;
       cout << 1 << ' ' << 1 << ' ' << N/2 << endl;
       cout << N/2 +1 << ' ' << 1 << ' ' << N/2 << endl;
       cout << 1 << ' ' << N/2 +1<< ' ' << N/2 << endl;
       cout << N/2+1 << ' ' << N/2+1 << ' ' << N/2 << endl;
       
    }
    
   printf("%d\n", q );
    return 0;
}