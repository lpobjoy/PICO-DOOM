# WAD File Setup Guide

## What is a WAD file?

WAD (Where's All the Data) files contain all the game data for Doom - levels, sprites, textures, sounds, music, etc. You need a WAD file to play Doom on your Pico.

## Legal WAD Files

### 1. Shareware DOOM1.WAD (Recommended)

The shareware version of Doom is **free and legal** to download and use.

**Size**: ~4.2 MB  
**Content**: Episode 1 (9 levels)  
**Download**: https://distro.ibiblio.org/slitaz/sources/packages/d/doom1.wad

This is the recommended option for Raspberry Pi Pico (2MB flash) as it fits with compression.

### 2. Ultimate Doom (DOOM.WAD)

**Size**: ~12 MB  
**Content**: All 4 episodes (36 levels)  
**Requirements**: 8MB+ flash device  
**Purchase**: Available on Steam, GOG, etc.

### 3. DOOM II (DOOM2.WAD)

**Size**: ~14 MB  
**Content**: 32 levels + secret levels  
**Requirements**: 8MB+ flash device  
**Purchase**: Available on Steam, GOG, etc.

## Installing a WAD File

### Option 1: External WAD (Recommended for Development)

1. Create the `wad` directory in your project root:
   ```bash
   mkdir -p wad
   ```

2. Copy your WAD file there:
   ```bash
   cp /path/to/doom1.wad wad/
   ```

3. The build system will look for WAD files in this directory.

**Note**: WAD files are in `.gitignore` and won't be committed to git.

### Option 2: Embedded WAD (For Standalone Builds)

To embed the WAD in flash memory:

1. Place your WAD in the `wad/` directory

2. Modify `CMakeLists.txt` to embed it:
   ```cmake
   # Add WAD embedding
   pico_set_binary_info(pico_doom
       PROGRAM_NAME "PICO-DOOM"
       PROGRAM_DESCRIPTION "Doom on Raspberry Pi Pico"
   )
   
   # Embed WAD file
   target_link_options(pico_doom PRIVATE
       -Wl,--embed-file,${CMAKE_CURRENT_SOURCE_DIR}/wad/doom1.wad
   )
   ```

3. Build normally - the WAD will be included in the UF2 file

## WAD File Compression

PicoDoom uses a custom `.whd` format that compresses the WAD to fit in limited flash:

1. Build the `whd_gen` tool:
   ```bash
   cd lib/picodoom
   mkdir build-native
   cd build-native
   cmake ..
   make whd_gen
   ```

2. Convert your WAD:
   ```bash
   ./whd_gen /path/to/doom1.wad output.whd
   ```

3. Use the `.whd` file in your build

**Compression ratios**:
- DOOM1.WAD: ~4.2 MB → ~2.8 MB (fits on Pico!)
- DOOM.WAD: ~12 MB → ~8 MB (needs 8MB+ flash)

## Verifying Your WAD

After placing your WAD file, you can verify it:

```bash
# Check file size
ls -lh wad/doom1.wad

# Calculate MD5 (optional, for verification)
md5sum wad/doom1.wad
```

**Known good MD5 checksums**:
- DOOM1.WAD (shareware): `5b2e249b9c5133ec987b3ea77596381d`
- DOOM.WAD (v1.9): `1cd63c5ddff1bf8ce844237f580e9cf3`
- DOOM2.WAD (v1.9): `25e1459ca71d321525f84628f45ca8cd`

## Troubleshooting

### "WAD file not found" error
- Ensure the file is in `wad/` directory
- Check filename matches exactly (case-sensitive on Linux/macOS)
- Verify file isn't corrupted (re-download if needed)

### "Out of memory" when loading
- You're trying to use a WAD that's too large for your flash
- Use DOOM1.WAD for 2MB Pico
- Upgrade to 8MB flash device for larger WADs
- Consider using WAD compression

### Game crashes after loading
- WAD file may be corrupted
- Try re-downloading the WAD
- Verify MD5 checksum
- Check you have enough RAM (close other tasks)

## Where to Get WAD Files

### Free/Shareware
- **DOOM1.WAD**: https://distro.ibiblio.org/slitaz/sources/packages/d/doom1.wad
- **FreeDoom**: https://freedoom.github.io/ (free alternative)

### Purchase
- **Steam**: https://store.steampowered.com/app/2280/
- **GOG**: https://www.gog.com/game/doom_ii
- **Bethesda**: https://bethesda.net/

## Legal Notes

- Shareware DOOM1.WAD is free to distribute
- Commercial WADs require purchase
- Never distribute commercial WADs without permission
- This project only includes the engine, not game data
- Respect id Software's intellectual property

---

**Ready to play?** Once you have your WAD file set up, proceed with building the project! See [BUILDING.md](BUILDING.md) for instructions.
