# LSM-KV: A Log-Structured Merge-Tree Key-Value Store

> Built from scratch in C++ as a hands-on companion to reading *Designing Data-Intensive Applications* (Kleppmann).

---

## Overview

LSM-KV is a persistent, embedded key-value storage engine based on the **Log-Structured Merge-Tree (LSM-tree)** architecture, the same core design powering production systems like LevelDB, RocksDB and Cassandra.

The goal of this project is to translate LSM-tree theory directly into working code: write path buffering, sorted immutable files, leveled compaction, crash recovery and more.

---

## Features

- **MemTable**: in-memory write buffer backed by a sorted structure for O(log n) reads and writes
- **SSTables**: immutable, sorted on-disk files flushed from the MemTable
- **Leveled Compaction**: multi-level compaction strategy to bound read amplification and reclaim space
- **Write-Ahead Log (WAL)**: append-only log ensuring durability and crash safety
- **Recovery**: full MemTable reconstruction from WAL on startup after a clean or unclean shutdown

---

## Architecture

```
Write Path
──────────
  put(k, v) ──► WAL (append) ──► MemTable (sorted, in-memory)
                                       │
                              [MemTable full / flush]
                                       │
                                       ▼
                              SSTable (Level 0, disk)
                                       │
                             [Compaction triggered]
                                       ▼
                           Level 1 → Level 2 → ... (disk)

Read Path
─────────
  get(k) ──► MemTable ──► L0 SSTables ──► L1 ──► L2 ──► ...
               (newest first, stop at first hit)
```

### Components

| Component | Responsibility |
|-----------|---------------|
| `MemTable` | In-memory sorted write buffer; serves reads before flush |
| `WAL` | Append-only log for durability; replayed on recovery |
| `SSTable` | Immutable sorted file on disk; produced by flushes and compaction |
| `ManifestManager` | Tracks which SSTable files exist per level; owns recovery state |
| `Compactor` | Implements leveled compaction; merges and rewrites SSTables across levels |

---

## Getting Started

### Prerequisites

- C++23 compiler
- CMake 3.15+

### Build

```bash
git clone git@github.com:Maher-Majdoub/lsm-kv.git
cd lsm-kv
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Usage

```cpp
#include "lsm/db/db.h"

lsm::DB db;

db.set("user:1", "alice");
db.set("user:2", "bob");

auto val = db.get("user:1");  // returns std::optional<std::string>
if (val) {
    std::cout << *val << "\n";  // alice
}

db.remove("user:2");

// get on a missing or removed key returns std::nullopt
assert(!db.get("user:2"));
```

---

## Design Notes

**Why LSM over B-Trees?**
LSM-trees convert random writes into sequential I/O by buffering mutations in memory and flushing sorted batches to disk. This makes them well-suited for write-heavy workloads. The tradeoff is read and space amplification, which compaction addresses over time.

**Leveled Compaction**
Each level has a size budget. When a level fills up, overlapping SSTables are merged into the next level, producing a single sorted run per level (L1+). This bounds read amplification to O(levels) and reclaims space from deleted/overwritten keys.

**Crash Recovery**
On startup, the WAL is replayed in order to reconstruct the MemTable as it existed before shutdown. The Manifest is used to recover the set of valid SSTable files per level, skipping any files not committed to a version.

---

## Roadmap

- [ ] **Bloom filters**: per-SSTable probabilistic filters to skip unnecessary disk reads on `get`
- [ ] **Concurrent compaction**: background compaction thread, decoupled from the write path
- [ ] **Concurrent MemTable flush**: non-blocking flush using an immutable MemTable double-buffer
- [ ] **Block cache**: LRU cache for frequently accessed SSTable blocks
- [ ] **Snapshot reads**: consistent point-in-time reads via sequence numbers
- [ ] **Compression**: block-level compression (Snappy / zstd) for SSTable data blocks

---

## Further Reading

This project was built alongside:

- *Designing Data-Intensive Applications*: Martin Kleppmann (Chapter 3)
- [RocksDB Wiki](https://github.com/facebook/rocksdb/wiki)
- [LevelDB Implementation Notes](https://github.com/google/leveldb/blob/main/doc/impl.md)

---

## License

MIT