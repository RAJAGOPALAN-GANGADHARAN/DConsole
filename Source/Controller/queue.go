package main

// type Queue struct {
// 	sharedSpace chan Message
// }

var channels map[string]chan Message

var memoryStash map[string][]Message
