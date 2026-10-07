-- Exclude Japanese words from buffer completion, including mixed Japanese/ASCII words.
local japanese = vim.regex("[々〆〇ぁ-ヿ㐀-䶿一-鿿豈-﫿ｦ-ﾝ]")

return {
  {
    "saghen/blink.cmp",
    opts = {
      sources = {
        providers = {
          buffer = {
            transform_items = function(_, items)
              return vim.tbl_filter(function(item)
                return japanese:match_str(item.label) == nil
              end, items)
            end,
          },
        },
      },
    },
  },
}
