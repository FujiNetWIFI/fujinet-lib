#!/usr/bin/env python3
# displays information about the specified apple single file.

import sys
import struct
from typing import List, Union, Tuple, Optional
from dataclasses import dataclass
from enum import IntEnum
from datetime import datetime

class ASEntryType(IntEnum):
    DATA_FORK = 1
    RESOURCE_FORK = 2
    REAL_NAME = 3
    COMMENT = 4
    ICON_BW = 5
    ICON_COLOR = 6
    FILE_DATES_INFO = 8
    FINDER_INFO = 9
    MACINTOSH_FILE_INFO = 10
    PRODOS_FILE_INFO = 11
    MSDOS_FILE_INFO = 12
    AFP_SHORT_NAME = 13
    AFP_FILE_INFO = 14
    AFP_DIRECTORY_ID = 15

@dataclass
class Point:
    x: int
    y: int

@dataclass
class FinderInfo:
    file_type: bytes
    file_creator: bytes
    flags: int
    location: Point
    folder_id: int

@dataclass
class ProDosInfo:
    access: int
    file_type: int
    auxiliary_type: int

@dataclass
class DataFork:
    data: bytes

@dataclass
class ResourceFork:
    data: bytes

@dataclass
class RealName:
    name: str

@dataclass
class Comment:
    text: str

@dataclass
class IconBW:
    data: bytes

@dataclass
class IconColor:
    data: bytes

@dataclass
class FileDatesInfo:
    creation_date: datetime
    modification_date: datetime
    backup_date: datetime
    access_date: datetime

@dataclass
class MacintoshFileInfo:
    data: bytes

@dataclass
class MSDOSFileInfo:
    data: bytes

@dataclass
class AFPShortName:
    name: str

@dataclass
class AFPFileInfo:
    data: bytes

@dataclass
class AFPDirectoryID:
    id: int

class AppleSingle:
    def __init__(self, file_path: str):
        with open(file_path, 'rb') as f:
            self.bytes = f.read()
        
        self.entries: List[Union[DataFork, ResourceFork, RealName, Comment, IconBW, 
                               IconColor, FileDatesInfo, FinderInfo, MacintoshFileInfo,
                               ProDosInfo, MSDOSFileInfo, AFPShortName, AFPFileInfo,
                               AFPDirectoryID]] = []
        self.load_address: int = 0
        
        self._parse()
    
    def _parse(self):
        # Check magic number (00051600 or 00051607)
        if len(self.bytes) < 4:
            raise ValueError("File too short for magic number")
        
        magic = struct.unpack('>I', self.bytes[0:4])[0]
        if magic not in [0x00051600, 0x00051607]:
            header_hex = self.bytes[0:4].hex()
            raise ValueError(f"Invalid magic number: {header_hex}")
        
        # Check version number (00020000)
        if len(self.bytes) < 8 or self.bytes[4:8] != b'\x00\x02\x00\x00':
            version_hex = self.bytes[4:8].hex()
            raise ValueError(f"Unsupported version: {version_hex}")
        
        # Check filler (16 bytes of zeros)
        if len(self.bytes) < 24:
            raise ValueError("File too short for header")
        if any(self.bytes[8:24]):
            raise ValueError("Invalid filler bytes")
        
        # Get number of entries (2 bytes, big-endian)
        if len(self.bytes) < 26:
            raise ValueError("File too short for entry count")
        num_entries = struct.unpack('>H', self.bytes[24:26])[0]
        
        # Parse entries
        offset = 26  # Start after header
        for _ in range(num_entries):
            if offset + 12 > len(self.bytes):
                raise ValueError("File too short for entry descriptor")
            
            entry_id = struct.unpack('>I', self.bytes[offset:offset+4])[0]
            entry_offset = struct.unpack('>I', self.bytes[offset+4:offset+8])[0]
            entry_length = struct.unpack('>I', self.bytes[offset+8:offset+12])[0]
            
            if entry_offset + entry_length > len(self.bytes):
                raise ValueError(f"Entry extends beyond file: type {entry_id}")
            
            entry_data = self.bytes[entry_offset:entry_offset + entry_length]
            
            try:
                entry_type = ASEntryType(entry_id)
            except ValueError:
                raise ValueError(f"Unsupported entry type: {entry_id}")
            
            if entry_type == ASEntryType.DATA_FORK:
                self.entries.append(DataFork(data=entry_data))
            
            elif entry_type == ASEntryType.RESOURCE_FORK:
                self.entries.append(ResourceFork(data=entry_data))
            
            elif entry_type == ASEntryType.REAL_NAME:
                self.entries.append(RealName(name=entry_data.decode('utf-8')))
            
            elif entry_type == ASEntryType.COMMENT:
                self.entries.append(Comment(text=entry_data.decode('utf-8')))
            
            elif entry_type == ASEntryType.ICON_BW:
                self.entries.append(IconBW(data=entry_data))
            
            elif entry_type == ASEntryType.ICON_COLOR:
                self.entries.append(IconColor(data=entry_data))
            
            elif entry_type == ASEntryType.FILE_DATES_INFO:
                if entry_length != 16:
                    raise ValueError(f"File dates info must be 16 bytes, got {entry_length}")
                dates = struct.unpack('>IIII', entry_data)
                self.entries.append(FileDatesInfo(
                    creation_date=datetime.fromtimestamp(dates[0]),
                    modification_date=datetime.fromtimestamp(dates[1]),
                    backup_date=datetime.fromtimestamp(dates[2]),
                    access_date=datetime.fromtimestamp(dates[3])
                ))
            
            elif entry_type == ASEntryType.FINDER_INFO:
                if entry_length != 16:
                    raise ValueError(f"Finder info must be 16 bytes, got {entry_length}")
                finder_info = struct.unpack('>4s4sHHHH', entry_data)
                self.entries.append(FinderInfo(
                    file_type=finder_info[0],
                    file_creator=finder_info[1],
                    flags=finder_info[2],
                    location=Point(x=finder_info[3], y=finder_info[4]),
                    folder_id=finder_info[5]
                ))
            
            elif entry_type == ASEntryType.PRODOS_FILE_INFO:
                if entry_length != 8:
                    raise ValueError(f"ProDOS info must be 8 bytes, got {entry_length}")
                access, file_type, auxiliary_type = struct.unpack('>HHI', entry_data)
                self.load_address = auxiliary_type & 0xFFFF
                self.entries.append(ProDosInfo(
                    access=access,
                    file_type=file_type,
                    auxiliary_type=auxiliary_type
                ))
            
            elif entry_type == ASEntryType.MACINTOSH_FILE_INFO:
                self.entries.append(MacintoshFileInfo(data=entry_data))
            
            elif entry_type == ASEntryType.MSDOS_FILE_INFO:
                self.entries.append(MSDOSFileInfo(data=entry_data))
            
            elif entry_type == ASEntryType.AFP_SHORT_NAME:
                self.entries.append(AFPShortName(name=entry_data.decode('utf-8')))
            
            elif entry_type == ASEntryType.AFP_FILE_INFO:
                self.entries.append(AFPFileInfo(data=entry_data))
            
            elif entry_type == ASEntryType.AFP_DIRECTORY_ID:
                if entry_length != 4:
                    raise ValueError(f"AFP directory ID must be 4 bytes, got {entry_length}")
                self.entries.append(AFPDirectoryID(id=struct.unpack('>I', entry_data)[0]))
            
            offset += 12
    
    def dump(self):
        print(f"AppleSingle, loadAddress: 0x{self.load_address:04x}")
        for entry in self.entries:
            if isinstance(entry, DataFork):
                up_to_7 = min(7, len(entry.data) - 1)
                first_8 = ' '.join(f"{b:02x}" for b in entry.data[:up_to_7+1])
                print(f"  DataForkEntry, len: 0x{len(entry.data):04x}: {first_8}")
            
            elif isinstance(entry, ResourceFork):
                up_to_7 = min(7, len(entry.data) - 1)
                first_8 = ' '.join(f"{b:02x}" for b in entry.data[:up_to_7+1])
                print(f"  ResourceForkEntry, len: 0x{len(entry.data):04x}: {first_8}")
            
            elif isinstance(entry, RealName):
                print(f"  RealName: {entry.name}")
            
            elif isinstance(entry, Comment):
                print(f"  Comment: {entry.text}")
            
            elif isinstance(entry, IconBW):
                print(f"  IconBW, len: 0x{len(entry.data):04x}")
            
            elif isinstance(entry, IconColor):
                print(f"  IconColor, len: 0x{len(entry.data):04x}")
            
            elif isinstance(entry, FileDatesInfo):
                print(f"  FileDatesInfo:")
                print(f"    Creation: {entry.creation_date}")
                print(f"    Modification: {entry.modification_date}")
                print(f"    Backup: {entry.backup_date}")
                print(f"    Access: {entry.access_date}")
            
            elif isinstance(entry, FinderInfo):
                print(f"  FinderInfo:")
                print(f"    Type: {entry.file_type.decode('ascii', errors='replace')}")
                print(f"    Creator: {entry.file_creator.decode('ascii', errors='replace')}")
                print(f"    Flags: 0x{entry.flags:04x}")
                print(f"    Location: ({entry.location.x}, {entry.location.y})")
                print(f"    Folder ID: 0x{entry.folder_id:04x}")
            
            elif isinstance(entry, ProDosInfo):
                print(f"  ProDosEntry, access: 0x{entry.access:04x}, fileType: 0x{entry.file_type:04x}, auxiliaryType: 0x{entry.auxiliary_type:08x}")
            
            elif isinstance(entry, MacintoshFileInfo):
                print(f"  MacintoshFileInfo, len: 0x{len(entry.data):04x}")
            
            elif isinstance(entry, MSDOSFileInfo):
                print(f"  MSDOSFileInfo, len: 0x{len(entry.data):04x}")
            
            elif isinstance(entry, AFPShortName):
                print(f"  AFPShortName: {entry.name}")
            
            elif isinstance(entry, AFPFileInfo):
                print(f"  AFPFileInfo, len: 0x{len(entry.data):04x}")
            
            elif isinstance(entry, AFPDirectoryID):
                print(f"  AFPDirectoryID: 0x{entry.id:08x}")

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <apple-single-file>")
        sys.exit(1)
    
    try:
        apple_single = AppleSingle(sys.argv[1])
        apple_single.dump()
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
