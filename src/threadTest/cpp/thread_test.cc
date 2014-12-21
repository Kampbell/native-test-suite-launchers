#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "gradle/thread.h"

using namespace testing;


class DefaultThread : public Test {
  public:
      void SetUp() {

      }

      void TearDown() {

      }
};

class DefaultThread_Create : public DefaultThread {};

TEST_F(DefaultThread_Create, null_thread_function__throw_kInvalidThreadFunction) {
    // Given:

    // When:
    ASSERT_THAT(gradle_thread_create(mock), Eq(kInvalidThreadFunction))
}
