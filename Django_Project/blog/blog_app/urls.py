from django.conf.urls import *
from blog_app.views import archive

urlpatterns = patterns('',
    url(r'^$', archive),
)
