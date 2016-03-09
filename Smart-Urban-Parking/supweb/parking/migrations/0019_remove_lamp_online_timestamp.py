# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0018_lamp_online_timestamp'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='lamp',
            name='online_timestamp',
        ),
    ]
