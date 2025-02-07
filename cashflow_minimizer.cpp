#include <bits/stdc++.h>
using namespace std;

struct Transaction {
    string payer, payee;
    double amount;
};

// Function to minimize cash flow
vector<Transaction> minimizeCashFlow(vector<Transaction>& transactions) {
    unordered_map<string, double> netBalance;

    // Calculate net balance for each entity
    for (size_t i = 0; i < transactions.size(); i++) {
        netBalance[transactions[i].payer] -= transactions[i].amount;
        netBalance[transactions[i].payee] += transactions[i].amount;
    }

    // Max heap for creditors (who should receive money)
    priority_queue<pair<double, string>> maxHeap;
    
    // Min heap for debtors (who owe money)
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> minHeap;

    // Store balances in heaps
    for (unordered_map<string, double>::iterator it = netBalance.begin(); it != netBalance.end(); it++) {
        if (it->second > 0)
            maxHeap.push(make_pair(it->second, it->first));
        else if (it->second < 0)
            minHeap.push(make_pair(-it->second, it->first));
    }

    vector<Transaction> minimizedTransactions;

    // Settling transactions optimally
    while (!minHeap.empty() && !maxHeap.empty()) {
        // Extract top debtor and creditor
        pair<double, string> minTop = minHeap.top(); minHeap.pop();
        double debt = minTop.first;
        string debtor = minTop.second;

        pair<double, string> maxTop = maxHeap.top(); maxHeap.pop();
        double credit = maxTop.first;
        string creditor = maxTop.second;

        double settleAmount = min(debt, credit);
        minimizedTransactions.push_back({debtor, creditor, settleAmount});

        // Adjust balances
        if (debt > credit)
            minHeap.push(make_pair(debt - credit, debtor));
        else if (credit > debt)
            maxHeap.push(make_pair(credit - debt, creditor));
    }

    return minimizedTransactions;
}

int main() {
    int numTransactions;
    cout << "\n*** Cashflow Minimizer ***\n";
    cout << "Enter number of transactions: ";
    cin >> numTransactions;

    vector<Transaction> transactions;
    
    cout << "Enter transactions in format: Payer Payee Amount\n";
    for (int i = 0; i < numTransactions; i++) {
        string payer, payee;
        double amount;
        cin >> payer >> payee >> amount;
        transactions.push_back({payer, payee, amount});
    }

    // Process cashflow minimization
    vector<Transaction> minimized = minimizeCashFlow(transactions);

    cout << "\n** Minimized Transactions **\n";
    for (size_t i = 0; i < minimized.size(); i++) {
        cout << minimized[i].payer << " pays " << minimized[i].amount << " to " << minimized[i].payee << endl;
    }

    return 0;
}
