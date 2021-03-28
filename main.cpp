#include <iostream>

extern void testWithIntegers() noexcept;
extern void testWithStrings() noexcept;

int main()
{
    std::cout << "=============== Проверка работы с числами ===============\n\n";
    testWithIntegers();

    std::cout << "\n=============== Проверка работы со строками ===============\n\n";
    testWithStrings();
    return 0;
}
