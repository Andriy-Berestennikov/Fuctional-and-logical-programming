#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    // Два масиви різної довжини, заповнені числами від 1 до N.
    // Кожен елемент масиву — це один розряд (цифра) числа.
    // array[0] — найстарший (лівий) розряд, останній елемент — наймолодший (правий).
    std::vector<int> array1(9);   // довжина 9: 1..9  -> число 123456789
    std::vector<int> array2(6);   // довжина 6: 1..6  -> число 123456

    for (size_t i = 0; i < array1.size(); ++i) array1[i] = static_cast<int>(i + 1);
    for (size_t i = 0; i < array2.size(); ++i) array2[i] = static_cast<int>(i + 1);

    size_t maxLen = std::max(array1.size(), array2.size());
    size_t totalDigits = maxLen + 1; // +1 місце про запас під можливий перенос у новий старший розряд

    // Вирівнюємо обидва числа по правому краю (доповнюємо нулями зліва),
    // щоб можна було додавати їх "стовпчиком" розряд за розрядом.
    // Кожен вектор має довжину totalDigits, з порожнім (нульовим) найстаршим розрядом.
    std::vector<int> a(totalDigits, 0), b(totalDigits, 0);
    std::copy(array1.rbegin(), array1.rend(), a.rbegin());
    std::copy(array2.rbegin(), array2.rend(), b.rbegin());

    std::vector<int> result(totalDigits, 0);
    std::vector<int> carry(totalDigits, 0); // carry[i] — перенос ІЗ розряду i у розряд (i-1)

    // Класичне додавання стовпчиком: справа наліво, з переносом
    int c = 0;
    for (size_t i = totalDigits; i-- > 0; ) {
        int sum = a[i] + b[i] + c;
        result[i] = sum % 10;
        c = sum / 10;
        if (i > 0) carry[i - 1] = c; // перенос, що приходить у наступний (лівіший) розряд
    }

    const int W = 4;         // ширина колонки під одну цифру
    const int LABEL_W = 11;  // ширина текстового підпису зліва

    // Скільки провідних розрядів є "порожніми" (не належать самому числу) —
    // потрібно, щоб не друкувати зайві нулі зліва.
    size_t emptyA = totalDigits - array1.size();
    size_t emptyB = totalDigits - array2.size();

    auto printLabel = [&](const std::string& text) {
        std::cout << std::setw(LABEL_W) << text;
        };

    std::cout << "=== Dodavannia stovpchykom ===\n\n";

    // --- Рядок переносів (показуємо лише там, де перенос > 0) ---
    printLabel("Perenos:");
    for (size_t i = 0; i < totalDigits; ++i) {
        std::cout << std::setw(W) << (carry[i] > 0 ? std::to_string(carry[i]) : "");
    }
    std::cout << "\n\n";

    // --- Перше число ---
    printLabel("Chyslo 1:");
    for (size_t i = 0; i < totalDigits; ++i) {
        if (i < emptyA) std::cout << std::setw(W) << ' ';
        else            std::cout << std::setw(W) << a[i];
    }
    std::cout << "\n";

    // --- Друге число зі знаком "+" ---
    printLabel("+ Chyslo 2:");
    for (size_t i = 0; i < totalDigits; ++i) {
        if (i < emptyB) std::cout << std::setw(W) << ' ';
        else            std::cout << std::setw(W) << b[i];
    }
    std::cout << "\n";

    // --- Лінія-роздільник ---
    std::cout << std::string(LABEL_W + static_cast<int>(totalDigits) * W, '-') << "\n";

    // --- Результат (провідний нуль, якщо переносу в новий розряд не було, не друкуємо) ---
    printLabel("Suma:");
    bool sumHasExtraDigit = (result[0] != 0);
    for (size_t i = 0; i < totalDigits; ++i) {
        if (i == 0 && !sumHasExtraDigit) std::cout << std::setw(W) << ' ';
        else                             std::cout << std::setw(W) << result[i];
    }
    std::cout << "\n";

    return 0;
}