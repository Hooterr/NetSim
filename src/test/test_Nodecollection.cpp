//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "factory.hpp"
#include "nodes.hpp"

// DEBUG
#include <iostream>

TEST(NodeCollectionTest, IsAddCorrect) {
    NodeCollection<Ramp> nc;
    Ramp r1 = Ramp(1, 1);
    Ramp r2 = Ramp(2, 1);
    Factory factory;
    nc.add(r1);
    nc.add(r2);
    EXPECT_EQ(std::distance(nc.cbegin(), nc.cend()), 2);
}

TEST(NodeCollectionTest, FindByIdCorrect) {
    NodeCollection<Ramp> nc;
    Ramp r1 = Ramp(1, 1);
    Ramp r2 = Ramp(2, 1);
    Factory factory;
    nc.add(r1);
    nc.add(r2);
    EXPECT_EQ(nc.find_by_id(2)->get_id(), 2);
}
TEST(NodeCollectionTest, RemoveByIdNoItemFound) {
    NodeCollection<Ramp> nc;
    Ramp r1 = Ramp(1, 1);
    Ramp r2 = Ramp(2, 1);
    Factory factory;
    nc.add(r1);
    nc.add(r2);
    nc.remove_by_id(3);
    GTEST_SUCCEED();
}

TEST(NodeCollectionTest, RemoveByIdCorrect) {
    NodeCollection<Ramp> nc;
    Ramp r1 = Ramp(1, 1);
    Ramp r2 = Ramp(2, 1);
    Factory factory;
    nc.add(r1);
    nc.add(r2);
    nc.remove_by_id(2);
    EXPECT_EQ(nc.find_by_id(2), nc.cend());
}

TEST(NodeCollectionTest, RemoveByIdNotPresent) {
    NodeCollection<Ramp> nc;
    Ramp r1 = Ramp(1, 1);
    Ramp r2 = Ramp(2, 1);
    Factory factory;
    nc.add(r1);
    nc.add(r2);
    nc.remove_by_id(3);
    EXPECT_EQ(nc.find_by_id(2)->get_id(), 2);
}