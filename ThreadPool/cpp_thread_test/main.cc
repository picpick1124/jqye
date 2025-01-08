#include "ThreadPool.hh"
#include <iostream>
#include <memory>
#include "test.hh"
#include <chrono>


int main()
{
    ThreadPool pool(12);

#ifdef FUNCTION_TEST
    // std::cout << "\n\n === The test for function === \n\n";
    function_test( pool );

#endif

	std::cout << "============== test0 ============== " << std::endl;
// #ifdef CLASS_TEST
//     // std::cout << "\n\n=== The test for class === \n\n";
// 	class_test( pool );
// #endif
// #ifdef LAMBDA_TEST
//     // std::cout << "\n\n === The test for lambda === \n\n";
//     lambda_test( pool );
// #endif

// 	std::this_thread::sleep_for(std::chrono::seconds(5));

//     std::cout << "\n\n ===the test in main=== \n\n";

	std::cout << "============== test1 ============== " << std::endl;

    // int result;
    // for (int i = 1; i <= 3; ++i)
    //     for (int j = 1; j <= 10; ++j)
    //     {
    //         pool.submit(multiply, std::ref(result), i, j);
    //     }

    return 0;
}
