return {
  {
    "lervag/vimtex",
    -- Newer releases require Neovim 0.12.4; this machine runs 0.12.3.
    tag = "v2.17",
    init = function()
      vim.g.vimtex_view_method = "zathura"
    end,
  },
}
