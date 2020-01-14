//
// Created by lsriw on 1/14/20.
//

#include "gtest/gtest.h"

#include "reports.hpp"

TEST(IntervalReportNotifierTest, ShouldGenerateRaportOK) {

    IntervalReportNotifier nt(2);
    EXPECT_TRUE(nt.should_generate_report(2));
}

TEST(IntervalReportNotifierTest, ShouldGenerateRaportNotGenerate) {

    IntervalReportNotifier nt(2);
    EXPECT_FALSE(nt.should_generate_report(1));
}

TEST(SpecificTurnsReportNotifierTest, ShouldGenerateRaportOK) {

    SpecificTurnsReportNotifier nt(std::set<Time>{1, 2});
    EXPECT_TRUE(nt.should_generate_report(2));
}

TEST(SpecificTurnsReportNotifierTest, ShouldGenerateRaportNotGenerate) {

    SpecificTurnsReportNotifier nt(std::set<Time>{2});
    EXPECT_FALSE(nt.should_generate_report(1));
}

