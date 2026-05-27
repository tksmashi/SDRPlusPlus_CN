#!/bin/bash
# ============================================================================
# SDR++ 汉化项目 - 上游同步脚本
# 用法: bash scripts/sync_upstream.sh [check|merge]
#   check  - 仅检查上游更新，不合并（默认）
#   merge  - 合并上游更新到当前分支
# ============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
UPSTREAM_URL="https://github.com/AlexandreRouma/SDRPlusPlus.git"
MANIFEST="$PROJECT_DIR/scripts/localized_files.txt"
MODE="${1:-check}"

cd "$PROJECT_DIR"

echo "============================================"
echo "  SDR++ 汉化项目 - 上游同步 ($MODE 模式)"
echo "============================================"
echo ""

# ---- 1. 确保 upstream remote 已配置 ----
if ! git remote | grep -q "^upstream$"; then
    echo "📡 添加 upstream remote: $UPSTREAM_URL"
    git remote add upstream "$UPSTREAM_URL"
else
    echo "📡 upstream remote 已存在"
fi

# ---- 2. 拉取上游代码 ----
echo "📡 拉取上游代码..."
if ! git fetch upstream 2>&1; then
    echo "❌ 无法拉取上游代码，请检查网络连接"
    exit 1
fi

# ---- 3. 检查新提交 ----
UPSTREAM_BRANCH="upstream/master"
# 检测上游默认分支名（可能是 master 或 main）
if ! git rev-parse --verify "$UPSTREAM_BRANCH" >/dev/null 2>&1; then
    UPSTREAM_BRANCH="upstream/main"
fi

NEW_COMMITS=$(git log HEAD.."$UPSTREAM_BRANCH" --oneline 2>/dev/null || echo "")
if [ -z "$NEW_COMMITS" ]; then
    echo "✅ 无上游更新，本地已是最新"
    echo ""
    echo "RESULT=NO_UPDATE"
    exit 0
fi

COMMIT_COUNT=$(echo "$NEW_COMMITS" | wc -l | tr -d ' ')
echo "📊 发现 $COMMIT_COUNT 个上游新提交:"
echo "$NEW_COMMITS" | head -20
if [ "$COMMIT_COUNT" -gt 20 ]; then
    echo "... 以及另外 $((COMMIT_COUNT - 20)) 个提交"
fi
echo ""

# ---- 4. 找出受影响的汉化文件 ----
CHANGED_FILES=$(git diff HEAD.."$UPSTREAM_BRANCH" --name-only)

if [ ! -f "$MANIFEST" ]; then
    echo "⚠️ 汉化文件清单不存在: $MANIFEST"
    AFFECTED_FILES=""
    AFFECTED_COUNT=0
else
    AFFECTED_FILES=""
    while IFS= read -r file; do
        # 跳过空行和注释行
        [[ -z "$file" || "$file" =~ ^# ]] && continue
        if echo "$CHANGED_FILES" | grep -qxF "$file"; then
            AFFECTED_FILES="${AFFECTED_FILES}${file}"$'\n'
        fi
    done < "$MANIFEST"
    AFFECTED_FILES=$(echo "$AFFECTED_FILES" | sed '/^$/d')
fi

if [ -z "$AFFECTED_FILES" ]; then
    echo "📋 上游有更新，但无汉化文件受影响"
    AFFECTED_COUNT=0
else
    AFFECTED_COUNT=$(echo "$AFFECTED_FILES" | wc -l | tr -d ' ')
    echo "📝 $AFFECTED_COUNT 个汉化文件受影响:"
    echo "$AFFECTED_FILES" | while IFS= read -r f; do
        echo "  - $f"
    done
fi
echo ""

# ---- 5. 如果只是检查，到此结束 ----
if [ "$MODE" = "check" ]; then
    echo "============================================"
    echo "  检查完成（check 模式，未合并）"
    echo "============================================"
    echo "RESULT=HAS_UPDATE"
    echo "UPSTREAM_COMMITS=$COMMIT_COUNT"
    echo "AFFECTED_LOCALIZED=$AFFECTED_COUNT"
    if [ -n "$AFFECTED_FILES" ]; then
        echo "NEEDS_RELOCALIZATION=1"
        echo "AFFECTED_FILE_LIST<<MANIFEST_EOF"
        echo "$AFFECTED_FILES" | while IFS= read -r f; do echo "$f"; done
        echo "MANIFEST_EOF"
    else
        echo "NEEDS_RELOCALIZATION=0"
    fi
    exit 0
fi

# ---- 6. 合并上游代码 ----
echo "🔀 合并上游代码..."
if git merge "$UPSTREAM_BRANCH" --no-edit; then
    echo "✅ 合并成功（无冲突）"
else
    echo "⚠️ 合并有冲突，自动解决中..."
    CONFLICTS=$(git diff --name-only --diff-filter=U)
    CONFLICT_COUNT=$(echo "$CONFLICTS" | wc -l | tr -d ' ')
    echo "  发现 $CONFLICT_COUNT 个冲突文件:"
    echo "$CONFLICTS" | while IFS= read -r f; do
        echo "  ⚔ $f → 采用上游版本"
    done
    # 对所有冲突文件采用上游版本（汉化文件后续会重新处理）
    for file in $CONFLICTS; do
        git checkout --theirs "$file"
        git add "$file"
    done
    git commit --no-edit
    echo "✅ 冲突已解决"
fi

echo ""
echo "============================================"
echo "  同步完成"
echo "============================================"
echo "  上游新提交:  $COMMIT_COUNT"
echo "  汉化文件受影响: $AFFECTED_COUNT"
echo "RESULT=MERGED"
echo "UPSTREAM_COMMITS=$COMMIT_COUNT"
echo "AFFECTED_LOCALIZED=$AFFECTED_COUNT"
if [ -n "$AFFECTED_FILES" ]; then
    echo "NEEDS_RELOCALIZATION=1"
    echo "AFFECTED_FILE_LIST<<MANIFEST_EOF"
    echo "$AFFECTED_FILES" | while IFS= read -r f; do echo "$f"; done
    echo "MANIFEST_EOF"
else
    echo "NEEDS_RELOCALIZATION=0"
fi
echo "============================================"
