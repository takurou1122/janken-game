#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include <ctime>

using namespace std;

// 制限時間（秒）
const int TIME_LIMIT = 3;

// プレイヤーの入力を非同期で取得するための変数
atomic<bool> input_received(false);
string player_choice;

void input_thread() {
    cout << "あなたの手を入力してください（グー/チョキ/パー）: ";
    getline(cin, player_choice);
    input_received = true;
}

string get_computer_choice() {
    string choices[] = {"グー", "チョキ", "パー"};
    return choices[rand() % 3];
}

string determine_winner(const string& player, const string& computer) {
    if (player == "timeout") return "lose";
    if (player == computer) return "draw";
    if ((player == "グー" && computer == "チョキ") ||
        (player == "チョキ" && computer == "パー") ||
        (player == "パー" && computer == "グー")) {
        return "win";
    }
    return "lose";
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "🎮 スピードモードじゃんけんゲームへようこそ！" << endl;
    cout << "⏳ 制限時間: " << TIME_LIMIT << "秒以内に手を入力してください。" << endl << endl;

    while (true) {
        input_received = false;
        player_choice = "";

        thread t(input_thread);

        for (int i = 0; i < TIME_LIMIT * 10; ++i) {
            if (input_received) break;
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        if (!input_received) {
            cout << "\n⏰ 時間切れ！入力がありませんでした。" << endl;
            player_choice = "timeout";
        }

        t.detach(); // 入力スレッドを切り離す（安全な終了のため）

        string computer_choice = get_computer_choice();
        if (player_choice != "timeout")
            cout << "🤖 相手の手: " << computer_choice << endl;

        string result = determine_winner(player_choice, computer_choice);

        if (result == "win") {
            cout << "✅ あなたの勝ち！" << endl;
        } else if (result == "lose") {
            cout << "❌ あなたの負け！" << endl;
        } else {
            cout << "🤝 引き分け！" << endl;
        }

        cout << "\nもう一度プレイしますか？（y/n）: ";
        string again;
        getline(cin, again);
        if (again != "y") {
            cout << "👋 ゲーム終了！ありがとうございました。" << endl;
            break;
        }

        cout << endl;
    }

    return 0;
}
