package main

import (
	"sync"
)

type QuickDeleteSlice struct {
	log_counts     []int64
	container_id   []string
	free_list      []int
	management_mut *sync.Mutex
}

func Make_QuickDeleteSlice() QuickDeleteSlice {
	retval := QuickDeleteSlice{}
	// default size is 300 because a single node is unlikely to have more than 300 containers. This way the data structure is unlikely to ever need
	// to stop and copy all the values.
	retval.log_counts = make([]int64, 0, 300)
	retval.container_id = make([]string, 0, 300)
	retval.free_list = make([]int, 0, 300)
	retval.management_mut = &sync.Mutex{}

	return retval
}

func (collection QuickDeleteSlice) get_free_index() int {
	collection.management_mut.Lock()
	defer collection.management_mut.Unlock()

	if len(collection.free_list) > 0 {
		free_index := collection.free_list[len(collection.free_list)-1]
		collection.free_list = collection.free_list[:len(collection.free_list)-1]
		collection.log_counts[free_index] = 0
		collection.container_id[free_index] = ""
		return free_index
	} else {
		collection.log_counts = append(collection.log_counts, 0)
		collection.container_id = append(collection.container_id, "")
		return len(collection.log_counts) - 1
	}
}

func (collection QuickDeleteSlice) remove_index(index int) {
	collection.management_mut.Lock()
	defer collection.management_mut.Unlock()

	collection.log_counts[index] = -1
	collection.container_id[index] = ""
	collection.free_list = append(collection.free_list, index)
}
