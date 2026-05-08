#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks/MockProductionRepository.h"
#include "Services/ProductionService.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Field;
using ::testing::Invoke;

TEST(ProductionServiceTest, Enqueue_CalculatesCorrectQty) {
    MockProductionRepository repo;
    ProductionJob captured;
    EXPECT_CALL(repo, SaveJob(_))
        .WillOnce(Invoke([&captured](const ProductionJob& j) {
            captured = j; return j;
        }));

    ProductionService svc(repo);
    // shortage=10, yield=0.8, avgTime=30
    // actual_qty = ceil(10 / (0.8 * 0.9)) = ceil(13.89) = 14
    // total_time = 30 * 14 = 420
    svc.Enqueue(1, 1, 10, 0.8, 30);

    EXPECT_EQ(captured.target_qty, 14);
    EXPECT_EQ(captured.total_time_min, 420);
}

TEST(ProductionServiceTest, Enqueue_BoundaryYieldOne) {
    MockProductionRepository repo;
    ProductionJob captured;
    EXPECT_CALL(repo, SaveJob(_))
        .WillOnce(Invoke([&captured](const ProductionJob& j) {
            captured = j; return j;
        }));

    ProductionService svc(repo);
    // shortage=1, yield=1.0, avgTime=10
    // actual_qty = ceil(1 / (1.0 * 0.9)) = ceil(1.11) = 2
    svc.Enqueue(1, 1, 1, 1.0, 10);

    EXPECT_EQ(captured.target_qty, 2);
}

TEST(ProductionServiceTest, Tick_StartsNextQueuedJob) {
    MockProductionRepository repo;

    ProductionJob queued;
    queued.id = 1; queued.total_time_min = 100; queued.elapsed_min = 0;
    queued.status = JobStatus::QUEUED;

    EXPECT_CALL(repo, FindRunningJob()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(repo, FindNextQueued()).WillOnce(Return(queued));
    EXPECT_CALL(repo, UpdateJob(_)).Times(2);  // 1: RUNNING 전환, 2: elapsed 갱신

    ProductionService svc(repo);
    svc.Tick();
}

TEST(ProductionServiceTest, Tick_NoJobDoesNothing) {
    MockProductionRepository repo;
    EXPECT_CALL(repo, FindRunningJob()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(repo, FindNextQueued()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(repo, UpdateJob(_)).Times(0);

    ProductionService svc(repo);
    svc.Tick();
}

TEST(ProductionServiceTest, Tick_CompletionFiresCallback) {
    MockProductionRepository repo;

    ProductionJob running;
    running.id = 1; running.order_id = 10; running.sample_id = 2;
    running.target_qty = 5; running.total_time_min = 10; running.elapsed_min = 0;
    running.start_time = std::time(nullptr) - 10;  // 10초 전에 시작 → elapsed >= total_time_min
    running.status = JobStatus::RUNNING;

    EXPECT_CALL(repo, FindRunningJob()).WillOnce(Return(running));
    EXPECT_CALL(repo, UpdateJob(Field(&ProductionJob::status, JobStatus::COMPLETED)));

    bool callbackFired = false;
    ProductionService svc(repo);
    svc.SetOnCompleteCallback([&callbackFired](const ProductionJob&) {
        callbackFired = true;
    });

    svc.Tick();  // elapsed = time(now) - start_time(10초전) = 10 >= total_time_min(10) → 완료
    EXPECT_TRUE(callbackFired);
}
