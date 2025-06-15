#!/usr/bin/env python3
import os
import urllib.request

# 下载文件
urls = [
    ("https://raw.githubusercontent.com/LingNc/Words_Counter/files/tests/HarryPotter.txt", "HarryPotter.txt"),
    ("https://raw.githubusercontent.com/LingNc/Words_Counter/refs/heads/files/tests/%C3%80LaRechercheDuTempsPerdu.txt", "ÀLaRechercheDuTempsPerdu.txt"),
    ("https://raw.githubusercontent.com/LingNc/Words_Counter/refs/heads/files/tests/FranceInEighteenHundredAndTwoDescribedInASeriesOfContemporaryLetters.txt", "FranceInEighteenHundredAndTwoDescribedInASeriesOfContemporaryLetters.txt"),
]

for url, filename in urls:
    if os.path.exists(filename):
        print(f"{filename} 已存在，跳过下载...")
        continue

    attempts = 0
    max_attempts = 3
    success = False

    while attempts < max_attempts and not success:
        attempts += 1
        try:
            print(f"下载 {filename}... (尝试 {attempts}/{max_attempts})")
            urllib.request.urlretrieve(url, filename)
            print(f"完成!")
            success = True
        except Exception as e:
            print(f"下载失败: {e}")
            if attempts >= max_attempts:
                print(f"达到最大尝试次数，无法下载 {filename}")
            else:
                print(f"正在重试...")

print("所有文件下载完成!")