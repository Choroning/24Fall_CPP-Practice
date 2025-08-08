/**
 * @file    A10204-1_C06Q1.cpp
 * @brief   Q1. 게시판
 * @author  박철원 (Cheolwon Park)
 * @date    2024-10-30
 */

#include <iostream>
#include <string>
#define MAX_NO 5        // 게시물의 최대 개수
using namespace std;

class Board {
    static int no;       // 현재 게시물의 개수
    static string board[MAX_NO];
    Board() {}           // private 접근 제어로 외부에서 객체 생성 방지
public:
    static void add(string t);
    static void del(int i);
    static void clear();
    static void print();
    static void modify(int no, string newtext);
};

// 각각의 멤버 함수 정의
void Board::add(string t) {
    if (no < MAX_NO) {
        board[no] = t;
        no++;
    } else {
        cout << "추가 실패! 게시물 개수가 " << MAX_NO << "개를 초과할 수 없습니다.\n" << endl;
    }
}

void Board::del(int i) {
    if ((board[i] != "") && (MAX_NO >= i)) {
        board[i] = "";
        
        if (i < no) {
            for (int j = i; j < no - 1; j++) {
                board[j] = board[j+1];
            }
            board[no] = "";
        }
        no--;
    } else {
        cout << "삭제 실패! 존재하지 않는 게시물입니다.\n" << endl;
    }
}

void Board::clear() {
    for (int i = 0; i < MAX_NO; i++) {
        if (board[i] != "") {
            board[i] = "";
        }
    }
    no = 0;
}

void Board::print() {
    int count = 0;
    cout << "********** [컴퓨터언어II 게시판] **********" << endl;
    
    for (int i = 0; i < no; i++) {
        cout << i << ": " << board[i] << endl;
        count++;
    }
    if (!count) {
        cout << "<등록된 게시물이 없습니다.>\n" << endl;
    }
    cout << endl;
}

void Board::modify(int no, string newtext) {
    if (no >= Board::no) {
        cout << "수정 실패! 존재하지 않는 게시물입니다.\n" << endl;
    } else {
        board[no] = newtext;
    }
}


// static 멤버 변수 외부 선언
int Board::no = 0;
string Board::board[MAX_NO] = {};


int main() {
    Board::add("PL센터 이용확인증 미제출자는 이번주까지 최소 1회 방문하시기 바랍니다.");
    Board::add("PL센터 이용확인증은 다음주까지 제출하세요.");
    Board::print();

    Board::add("6장 이론과제는 다음주 화요일까지 온라인 제출하세요.");
    Board::add("6장 실습과제는 이번주 토요일까지 온라인 제출하세요.");
    Board::add("수업시간에 확인받지 못한 경우에만 업로드하면 됩니다.");
    Board::print();

    // add() 오류 사례 - 게시물 최대 개수 초과
    Board::add("소스코드는 압축 없이 개별 파일로 업로드하세요.");
    Board::modify(3, "(수정) 6장 실습과제는 이번주 금요일까지 온라인 제출하세요.");
    Board::print();

    Board::del(5);       // del() 오류 사례 - 존재하지 않는 게시물 삭제 요청
    Board::del(2);
    Board::print();
    Board::del(0);
    Board::print();

    // modify() 오류 사례 - 존재하지 않는 게시물
    Board::modify(4, "프로그램 제출기한 엄수바랍니다.");
    Board::add("실습과제 수행 시 PL센터의 도움을 받을 수 있습니다.");
    Board::print();
    Board::clear();
    Board::print();

    return 0;
}
