# same as default setting of mrusby
MRuby::Build.new do |conf|
  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :clang
  end
  conf.gembox 'default'
  conf.bins = []
  conf.cc.defines = %w("MRB_METHOD_CACHE MRB_UTF8_STRING MRB_ENABLE_DEBUG")
end
