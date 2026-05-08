#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks/MockProductionRepository.h"
#include "Mocks/MockProductionLineRepository.h"
#include "Services/ProductionService.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Field;
using ::testing::Invoke;

TEST(ProductionServiceTest, Enqueue_CalculatesCorrectQty) {
    MockProductionRepository     repo;
    MockProductionLineRepository lineRepo;
    ProductionJob captured;
    EXPECT_CALL(repo, SaveJob(_))
        .WillOnce(Invoke([&captured](const ProductionJob& j) { captured = j; return j; }));
    EXPECT_CALL(repo, SaveToFile()).Times(::testing::AnyNumber());

    ProductionService svc(repo, lineRepo);
    // shortage=10, yield=0.8, avgTime=30 → actual_qty=14, total_time=420
    svc.Enqueue(1, 1, 10, 0.8, 30);
    EXPECT_EQ(captured.target_qty, 14);
    EXPECT_EQ(captured.total_time_min, 420);
    EXPECT_EQ(captured.line_id, 0);   // 미배정 확인
}

TEST(ProductionServiceTest, Enqueue_BoundaryYieldOne) {
    MockProductionRepository     repo;
    MockProductionLineRepository lineRepo;
    ProductionJob captured;
    EXPECT_CALL(repo, SaveJob(_))
        .WillOnce(Invoke([&captured](const ProductionJob& j) { captured = j; return j; }));
    EXPECT_CALL(repo, SaveToFile()).Times(::testing::AnyNumber());

    ProductionService svc(repo, lineRepo);
    // shortage=1, yield=1.0 → actual_qty=2
    svc.Enqueue(1, 1, 1, 1.0, 10);
    EXPECT_EQ(captured.target_qty, 2);
}

TEST(ProductionServiceTest, Tick_StartsNextQueuedJob) {
    MockProductionRepository     repo;
    MockProductionLineRepository lineRepo;

    ProductionJob queued;
    queued.id = 1; queued.total_time_min = 100; queued.line_id = 0;
    queued.status = JobStatus::QUEUED;

    std::vector<ProductionLine> lines = { {1, "Line-1", ""} };
    EXPECT_CALL(lineRepo, FindAll()).WillOnce(Return(lines));
    EXPECT_CALL(repo, FindRunningJobByLine(1)).WillOnce(Return(std::nullopt));
    EXPECT_CALL(repo, FindNextQueued()).WillOnce(Return(queued));
    EXPECT_CALL(repo, UpdateJob(_)).Times(2);
    EXPECT_CALL(repo, SaveToFile()).Times(1);

    ProductionService svc(repo, lineRepo);
    svc.Tick();
}

TEST(ProductionServiceTest, Tick_NoJobDoesNothing) {
    MockProductionRepository     repo;
    MockProductionLineRepository lineRepo;

    std::vector<ProductionLine> lines = { {1, "Line-1", ""} };
    EXPECT_CALL(lineRepo, FindAll()).WillOnce(Return(lines));
    EXPECT_CALL(repo, FindRunningJobByLine(1)).WillOnce(Return(std::nullopt));
    EXPECT_CALL(repo, FindNextQueued()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(repo, UpdateJob(_)).Times(0);
    EXPECT_CALL(repo, SaveToFile()).Times(1);

    ProductionService svc(repo, lineRepo);
    svc.Tick();
}

TEST(ProductionServiceTest, Tick_CompletionFiresCallback) {
    MockProductionRepository     repo;
    MockProductionLineRepository lineRepo;

    ProductionJob running;
    running.id = 1; running.order_id = 10; running.sample_id = 2; running.line_id = 1;
    running.target_qty = 5; running.total_time_min = 10; running.elapsed_min = 0;
    running.start_time = std::time(nullptr) - 10;
    running.status = JobStatus::RUNNING;

    std::vector<ProductionLine> lines = { {1, "Line-1", ""} };
    EXPECT_CALL(lineRepo, FindAll()).WillOnce(Return(lines));
    EXPECT_CALL(repo, FindRunningJobByLine(1)).WillOnce(Return(running));
    EXPECT_CALL(repo, UpdateJob(Field(&ProductionJob::status, JobStatus::COMPLETED)));
    EXPECT_CALL(repo, SaveToFile()).Times(1);

    bool callbackFired = false;
    ProductionService svc(repo, lineRepo);
    svc.SetOnCompleteCallback([&callbackFired](const ProductionJob&) { callbackFired = true; });
    svc.Tick();
    EXPECT_TRUE(callbackFired);
}
