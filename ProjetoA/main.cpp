#include <iostream>
#include <array>
#include <algorithm>
#include <math.h>
#include <vector>

//#define MOVE
//#define MOVES
//#define DEBUG

using namespace std;
int plays_left = -1;
const char *lista[4] = {"Esquerda", "Direita", "Cima", "Baixo"};

class matrix_pos{
private:
    int tam;
    bool dir;
    int lin;
    int curr_pos;
public:
    explicit matrix_pos(int tamanho, bool direcao, int linha){
        tam=tamanho;
        dir=direcao;//True = vertical ou False = Horizontal
        lin=linha;
        curr_pos=-1;
    }
    void reset(){
        curr_pos=-1;
    }
    int convert_pos(int n){
        curr_pos=n-1;
        if(dir){//Se for vertical
            return (++curr_pos*tam)+lin;
        }else{//Se for horizontal
            return (tam*lin)+(++curr_pos);
        }
    }
    int next(){
        if(curr_pos==-1){
            ++curr_pos;
            if(dir){
                return lin;
            }else{
                return lin*tam;
            }
        }

        if(dir){//Se for vertical
            return (++curr_pos*tam)+lin;
        }else{//Se for horizontal
            return (tam*lin)+(++curr_pos);
        }
    }
};


class Tabuleiro {
private:
    int tam; //Tamanho e jogadas
    int curr_pos;

    std::array<int, 4> last_move {}; //Left, Right, Up, Down
    std::array<int, 20*20> tabuleiro{};

    int check_joins(bool direcao,int coluna, bool sentido){

#ifdef DEBUG
      cout << "Linha/Coluna " <<coluna+1<<"\n";
#endif
        //DEBUG
        /*cout << "[" << n << "]\n";
        matrix_pos k(getTamanho(),direcao, n );
        for(int i=0; i < tam; i++){
            cout << tabuleiro[k.convert_pos(i)]<<" ";
        }
        cout<<"\n";*/

        int possible=0;
        //Primeiro vemos se é linha ou coluna que queremos
        //Arranjamos um iterador
        matrix_pos k(getTamanho(),direcao, coluna );

        if(sentido){
            for(int i=0; i<tam-1; ++i){
                int curr = i+1;

                int a=tabuleiro[k.convert_pos(i)];

                while(curr<tam){
                    int b = tabuleiro[k.convert_pos(curr)];

#ifdef DEBUG
                    //DEBUG
                    cout << "A->" << a << " B->" << b << "\n";
#endif

                    if(a==0 && b!=0){
                        tabuleiro[k.convert_pos(i)] = b;
                        a = b;
                        tabuleiro[k.convert_pos(curr)] = 0;
#ifdef DEBUG
                        print();//DEBUG
#endif
                        //++possible;
                        //break;
                    }else if(b==0){
                        ++curr;
                    }else if(a==b){
                        tabuleiro[k.convert_pos(i)] = a*2;
                        tabuleiro[k.convert_pos(curr)] = 0;

#ifdef DEBUG
                    print();//DEBUG
#endif
                        ++possible;
                        break;
                    }else{
                        break;
                    }
                }
            }
        }else{
            for(int i=tam-1; i>0; --i){
                int curr = i-1;

                int a=tabuleiro[k.convert_pos(i)];

                while(curr>=0){
                    int b = tabuleiro[k.convert_pos(curr)];

#ifdef DEBUG
                    //DEBUG
                    cout << "B->" << b << " A->" << a << "\n";
#endif

                    if(a==0 && b!=0){
                        tabuleiro[k.convert_pos(i)] = b;
                        a = b;
                        tabuleiro[k.convert_pos(curr)] = 0;
#ifdef DEBUG
                        print();//DEBUG
#endif
                        //++possible;
                        //break;
                    }else if(b==0){
                        --curr;
                    }else if(a==b){
                        tabuleiro[k.convert_pos(i)] = a*2;
                        tabuleiro[k.convert_pos(curr)] = 0;

#ifdef DEBUG
                        print();//DEBUG
#endif
                        ++possible;
                        break;
                    }else{
                        break;
                    }
                }
            }
        }
        return possible;
    }

public:
    explicit Tabuleiro(int tamanho){ //Construtor
        tam = tamanho;
        curr_pos = 0;
    }
    Tabuleiro(const Tabuleiro& antigo){
        std::array<int,20*20> a = antigo.tabuleiro;
        tabuleiro = a;
        last_move = antigo.last_move;
        tam = antigo.tam;
        curr_pos = antigo.curr_pos;
    }
    void ordenaNum(Tabuleiro& t2){
        sort(t2.tabuleiro.begin(),t2.tabuleiro.end(),greater<int>());
        int aux=0;
        for(int i=0; i<tam*tam; ++i){
            if(t2.tabuleiro[i]==0){
                break;
            }
            aux++;
        }
        sort(t2.tabuleiro.begin(),t2.tabuleiro.begin()+aux);
    }
    int notPossible(int jogadas){
        Tabuleiro t2 = *this;
        sort(t2.tabuleiro.begin(),t2.tabuleiro.end());
        int moves, count=-1;
        do{
            count++;
            moves=0;
            ordenaNum(t2);
            //t2.print();
            for(int i = 0; i < tam; i++){
                moves += t2.check_joins(false, i, true);
            }
        } while (moves>0);



        if(count<=jogadas and t2.getWon()){
            return 0;
        }else{
#ifdef MOVES
            cout << "\nJogadas que dão: "<<jogadas<<" Minimo possivel: "<<count<<"\n";
#endif
            return 1;
        }
    }

    int getTamanho() {
        return tam;
    }
    void add(int elem){
        tabuleiro[curr_pos++]=elem;
    }
    bool getWon(){ //-------------------------------------------------Pode ser mudada para improve performance
        int count = 0;
        for(int i=0; i < tam*tam; i++){
            if(tabuleiro[i]!=0){
                ++count;
            }
        }
        if(count==1){
            return true;
        }else{
            return false;
        }
    }
    int cannotMove(int i){
        return last_move[i];
    }
    bool allSet(){
        int aux=0;
        for(int i=0; i<4; ++i){
            if(last_move[i]>0)
                aux++;
        }
        if(aux==4){
            return true;
        }else{
            return false;
        }
    }
    void setCannotMove(int i){
        last_move[i]++;
    }
    void resetCannotMove(int i){
        last_move[i]=0;
    }
    /*void resetCannotMove(){
        last_move.fill(0);
    }*/
    void print(){
        for(int i=0; i < tam*tam; i++){
            if((i+1)%tam==0){
                cout << tabuleiro[i] << "\n";
            }else{
                cout << tabuleiro[i] << " ";
            }
        }
    }
    int check_move(int move){
        Tabuleiro t2 = *this;
        int moves=0;
        bool direcao=false, sentido=false;
        switch (move) {
            case 1://Esquerda
               direcao=false;
               sentido=true;
               break;
            case 2://Direita
                direcao=false;
                sentido=false;
                break;
            case 3://Cima
                direcao=true;
                sentido=true;
                break;
            case 4://Baixo
                direcao=true;
                sentido=false;
                break;
        }
        for(int i = 0; i < tam; i++){
            moves += t2.check_joins(direcao, i, sentido);
        }
        bool flag = false;
        for(int i=0; i<tam*tam; ++i){ //Ver se o tabuleiro ficou na mesma
            if(t2.tabuleiro[i]!=this->tabuleiro[i]){
                flag=true;
                break;
            }
        }
        if(!flag)
            return 2;
        else
            if(moves!=0)
                return 0;
            else
                return 1;
    }

    Tabuleiro slide_left(){
        Tabuleiro t2 = *this;
        int moves = 0;
        //Precisamos de ver as linhas
        for(int i = 0; i < tam; i++){
            moves += t2.check_joins(false, i, true);
        }
        /*if(moves==0){
            t2.setCannotMove(0);
            setCannotMove(0);
            t2.setCannotMove(1);//Direita
        }else{
            t2.resetCannotMove();
            resetCannotMove();
        }*/
#ifdef MOVE
        cout << "Slide Left\n";
        t2.print();
#endif
        return t2; //Retorno copia
    }
    Tabuleiro slide_right(){
        Tabuleiro t2 = *this;
        int moves = 0;
        //Precisamos de ver as linhas
        for(int i = 0; i < tam; i++){
            moves += t2.check_joins(false, i, false);
        }
        /*if(moves==0){
            t2.setCannotMove(1);
            setCannotMove(1);
            t2.setCannotMove(0);//Esquerda
        }else{
            t2.resetCannotMove();
            resetCannotMove();
        }*/
#ifdef MOVE
        cout << "Slide Right\n";
        t2.print();
#endif
        return t2;
    }
    Tabuleiro slide_up(){
        Tabuleiro t2 = *this;
        int moves = 0;
        //Precisamos de ver as colunas
        for(int i = 0; i < tam; i++){
            moves += t2.check_joins(true, i, true);
        }
        /*if(moves==0){
            t2.setCannotMove(2);
            setCannotMove(2);
            t2.setCannotMove(3);//Baixo
        }else{
            t2.resetCannotMove();
            resetCannotMove();
        }*/
#ifdef MOVE
        cout << "Slide Up\n";
        t2.print();
#endif
        return t2;
    }
    Tabuleiro slide_down(){
        Tabuleiro t2 = *this;
        int moves = 0;
        //Precisamos de ver as colunas
        for(int i = 0; i < tam; i++){
            moves += t2.check_joins(true, i, false);
        }
        /*if(moves==0){ //Conseguiu somar algum?
            t2.setCannotMove(3);
            setCannotMove(3);
            t2.setCannotMove(2);//Cima
        }else{
            t2.resetCannotMove();
            resetCannotMove();
        }*/
#ifdef MOVE
        cout << "Slide Down\n";
        t2.print();
#endif
        return t2;
    }
};
int chamadas;
int testarJogo(int plays, Tabuleiro jogo, array<int,4> move){
    chamadas++;
#ifdef MOVES
    for(int i=0; i<4; ++i){
        cout << move[i] << " ";
    }
    cout << "\n";
    jogo.print();
    cout << "\n";
#endif

    if(plays<=plays_left){ //Se já tiver ganho alguma vez e as minhas jogadas ja ultrapassarem
        return 0;
    }
    if(jogo.notPossible(plays)){ //Testa o melhor caso
        return 0;
    }
    if(jogo.getWon()){//Se estiver ganho? Sera que ganhei com menos jogadas
        if(plays>plays_left or plays_left==-1){
            plays_left=plays;
        }
#ifdef MOVES
        cout << "\nGanho com -----------------------> "<< plays <<" plays left\n";
#endif
        return 1;
    }
    if(plays<=0){
#ifdef MOVES
        cout << "\nNo solution on this path --------> "<< plays <<" plays left\n";
#endif
        return -1;
    }else{
#ifdef MOVES
        cout<<"["<<plays<<"] Esquerda ->";
        for(int i=0; i<4; ++i){
            cout << move[i] << " ";
        }
        cout << "\n";
#endif
        array<int,4> cpy = move;
        if(cpy[0]<2){ //Esquerda ja fiz?
            bool faz=true;
            Tabuleiro aux = jogo.slide_left();
            if(aux.check_move(1)==2) { //Ficou igual?
                move[0] = 2; //Sim
                move[1] = 2;
                move[2] = aux.check_move(3);
                move[3] = aux.check_move(4);
            }else if(aux.check_move(1)==1){ //Fez aglomeração?
                move[0] += 1; //One more chance
                move[1] += 1;
                move[2] += aux.check_move(3);
                move[3] += aux.check_move(4);
            }else{
                move[0] = 0;
                move[1] = 0;
                move[2] = aux.check_move(3);
                move[3] = aux.check_move(4);
                if(aux.getWon()){
                    if(plays>plays_left or plays_left==-1){
                        plays_left=plays-1;
                    }
                    faz=false;
                }
            }
            if(faz) {
#ifdef MOVES
                cout << "Esquerda-> ";
#endif
                testarJogo(plays - 1, jogo.slide_left(), move);
            }
        }

        if(cpy[1]<2){ //Direita ja fiz?
            move=cpy;
#ifdef MOVES
            cout<<"["<<plays<<"] Direita ->";
            for(int i=0; i<4; ++i){
                cout << move[i] << " ";
            }
            cout << "\n";
#endif
            bool faz=true;
            Tabuleiro aux = jogo.slide_right();
            if(aux.check_move(2)==2) { //Deu-me algum movimento?
                move[0] = 2;
                move[1] = 2; //Não
                move[2] = aux.check_move(3);
                move[3] = aux.check_move(4);
            }else if(aux.check_move(2)==1){ //Mudou alguma coisa? Sim
                move[0] += 1; //One more chance
                move[1] += 1;
                move[2] += aux.check_move(3);
                move[3] += aux.check_move(4);
            }else{
                move[0] = 0;
                move[1] = 0;
                move[2] = aux.check_move(3);
                move[3] = aux.check_move(4);
                if(aux.getWon()){
                    if(plays>plays_left or plays_left==-1){
                        plays_left=plays-1;
                    }
                    faz=false;
                }
            }
            if(faz) {
#ifdef MOVES
                cout << "Direita-> ";
#endif
                testarJogo(plays - 1, jogo.slide_right(), move);
            }
        }

        if(cpy[2]<2){ //Cima ja fiz?
            move=cpy;
#ifdef MOVES
            cout<<"["<<plays<<"] Cima ->";
            for(int i=0; i<4; ++i){
                cout << move[i] << " ";
            }
            cout << "\n";
#endif
            bool faz=true;
            Tabuleiro aux = jogo.slide_up();
            if(aux.check_move(3)==2) { //Deu-me algum movimento?
                move[0] = aux.check_move(1);
                move[1] = aux.check_move(2);
                move[2] = 2;
                move[3] = 2;

            }else if(aux.check_move(3)==1){ //Mudou alguma coisa? Sim
                move[0] += aux.check_move(1);
                move[1] += aux.check_move(2);
                move[2] += 1;
                move[3] += 1;
            }else{
                move[2] = 0;
                move[3] = 0;
                move[0] = aux.check_move(1);
                move[1] = aux.check_move(2);
                if(aux.getWon()){
                    if(plays>plays_left or plays_left==-1){
                        plays_left=plays-1;
                    }
                    faz=false;
                }
            }
            if(faz) {
#ifdef MOVES
                cout << "Cima-> ";
#endif
                testarJogo(plays - 1, jogo.slide_up(), move);
            }
        }

        if(cpy[3]<2){ //Baixo ja fiz?
            move=cpy;
#ifdef MOVES
            cout<<"["<<plays<<"] Baixo ->";
            for(int i=0; i<4; ++i){
                cout << move[i] << " ";
            }
            cout << "\n";
#endif
            bool faz=true;
            Tabuleiro aux = jogo.slide_down();
            if(aux.check_move(4)==2) { //Na mesma?
                move[0] = aux.check_move(1);
                move[1] = aux.check_move(2);
                move[2] = 2;
                move[3] = 2;

            }else if(aux.check_move(3)==1){ //Mudou alguma coisa? Nao
                move[0] += aux.check_move(1);
                move[1] += aux.check_move(2);
                move[2] += 1;
                move[3] += 1;
            }else{
                move[2] = 0;
                move[3] = 0;
                move[0] = aux.check_move(1);
                move[1] = aux.check_move(2);
                if(aux.getWon()){
                    if(plays>plays_left or plays_left==-1){
                        plays_left=plays-1;
                    }
                    faz=false;
                }
            }
            if(faz) {
#ifdef MOVES
                cout << "Baixo-> ";
#endif
                testarJogo(plays - 1, jogo.slide_down(), move);
            }
        }
        return 0;
    }
}

int verificaImpares(vector<int> valores){
    int size = valores.size();
    for(int i=0; i<size; ++i){
        if(valores[i]%2!=0){//Se for número ímpar
            return 0;
        }else if(i+1==size){//Se chegar ao fim
            if(valores[i]%2==0){
                return 1;
            }else{
                return 0;
            }
        }else{
            valores[i+1]+=valores[i]/2;
            valores[i]=0;
        }
    }
    return 0;
}

bool receberInput(){
    int n, tam, plays;
    int flag;

    cin >> n;
    for(int i=0; i < n; ++i){   //Numero de jogos
        plays_left=-1;
        flag=0;

        cin >> tam;     //Tamanho do tabuleiro
        cin >> plays;   //Numero de jogadas
        if(tam > 20){
            cout << "no solution\n";
            flag=1;
        }
        if(plays>50){
            cout << "no solution\n";
            flag=1;
        }
        int elem;
        Tabuleiro jogo(tam);    //Criar um novo tabuleiro

        //Verificação inicial
        vector<int> valores;

        for (int j = 0; j < tam*tam; ++j) {
            cin >> elem;
            if(flag==1)
                continue;
            else{
                jogo.add(elem);
                if(elem==0){

                }else if(valores.size()>=log2(elem)){
                    valores[log2(elem)-1]+=1;
                }else{
                    int size = valores.size();
                    for(int k=0; k<log2(elem)-size; k++){
                        if(k==log2(elem)-size-1)
                            valores.push_back(1);
                        else
                            valores.push_back(0);
                    }
                }
            }
        }

        if(flag==1){
            continue;
        }

        chamadas=0;
        if(verificaImpares(valores)==1){
            std::array<int, 4> moves {};
            testarJogo(plays, jogo, moves);
        }
        //cout<<"Chamadas->"<<chamadas<<"\n";

        if(plays_left!=-1)
            cout << plays-plays_left << "\n";
        else
            cout << "no solution\n";
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    receberInput();
    return 0;
}
